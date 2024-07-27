//
// Created by konstantin on 27.07.24.
//

#include <cassert>
#include <coroutine>
#include <iostream>
#include <unordered_map>

#include "generator.h"

struct Resumable final {
    struct promise_type {
        using coro_handle = std::coroutine_handle<promise_type>;

        size_t current_value{};

        std::suspend_always yield_value(const size_t pos) {
            current_value = pos;
            return {};
        }

        auto get_return_object() { return coro_handle ::from_promise(*this); }

        std::suspend_always initial_suspend() { return {}; }

        std::suspend_always final_suspend() noexcept { return {}; }

        auto unhandled_exception() { std::terminate(); }
    };

    Resumable(promise_type::coro_handle handle) : handle_(handle) {}

    ~Resumable() {
        if (handle_) handle_.destroy();
    }

    promise_type::coro_handle handle() {
        promise_type::coro_handle h = handle_;
        handle_ = nullptr;
        return h;
    }

    bool resume() {
        if (handle_) handle_.resume();
        return !handle_.done();
    }

private:
    promise_type::coro_handle handle_;
};

using coro_t = std::coroutine_handle<>;

enum class Sym : char { A, B, Term };
enum class State { A, B };

Generator<Sym> input_seq(std::string seq) {
    for (char c : seq) {
        switch (c) {
            case 'a':
                co_yield Sym::A;
                break;
            case 'b':
                co_yield Sym::B;
                break;
            default:
                co_yield Sym::Term;
                break;
        }
    }
    for (;;) {
        co_yield Sym::Term;
    }
}

template <typename F, typename SM>
struct stm_awaiter : public F {
    SM& stm;
    stm_awaiter(F f, SM& stm) : F(f), stm(stm) {}

    bool await_ready() const noexcept { return false; }
    coro_t await_suspend(std::coroutine_handle<>) noexcept {
        stm.gennext();
        auto sym = stm.genval();
        auto new_state = F::operator()(sym);
        return stm[new_state];
    }
    bool await_resume() const noexcept { return stm.genval() == Sym::Term; }
};

template <class State, class Sym>
class StateMachine final {
public:
    StateMachine(Generator<Sym> gen) : gen(std::move(gen)) {}

    coro_t operator[](State s) { return states[s]; }

    template <typename F>
    auto get_awaiter(F transition) {
        return stm_awaiter(transition, *this);
    }

    template <class F>
    void add_state(State state, F stf) {
        states[state] = stf(*this).handle();
    }

    void run(State initial) {
        current_state = initial;
        states[initial].resume();
    }

    Sym genval() const { return gen.current_value(); }

    void gennext() { gen.move_next(); }

    State current() const { return current_state; }

private:
    State current_state;
    std::unordered_map<State, coro_t> states;
    Generator<Sym> gen;
};

using stm_t = StateMachine<State, Sym>;

Resumable StateA(stm_t& stm) {
    auto transmition = [](auto sym) {
        if (sym == Sym::B) {
            return State::B;
        }
        return State::A;
    };
    for (;;) {
        std::cout << "State A" << std::endl;
        bool finish = co_await stm.get_awaiter(transmition);
        if (finish) {
            break;
        }
    }
}

Resumable StateB(stm_t& stm) {
    auto transmition = [](auto sym) {
        if (sym == Sym::A) {
            return State::A;
        }
        return State::B;
    };
    for (;;) {
        std::cout << "State B" << std::endl;
        bool finish = co_await stm.get_awaiter(transmition);
        if (finish) {
            break;
        }
    }
}

int main() {
    auto gen = input_seq("aaabbaba");
    stm_t stm(std::move(gen));
    stm.add_state(State::A, StateA);
    stm.add_state(State::B, StateB);

    stm.run(State::A);

    auto curr = stm.current();
    assert(curr == State::A);
}