//
// Created by konstantin on 13.07.24.
//

#include <cassert>
#include <coroutine>
#include <iostream>
#include <vector>

std::vector<std::string> messages{"Hello", "world", "!"};

struct Resumable {
    struct SuspendAlways final {
        bool await_ready() const noexcept { return false; }
        void await_suspend(std::coroutine_handle<> ) const noexcept { }
        void await_resume() const noexcept { }
    };

    struct promise_type {
        using coro_handle = std::coroutine_handle<promise_type>;

        size_t current_value{};

        std::suspend_always yield_value(const size_t pos) {
            std::cout << "[promise_type::yield_value] call with pos=" << pos
                      << ", current_value=" << current_value << std::endl;

            current_value = pos;
            return {};
        }

        auto get_return_object() { return coro_handle ::from_promise(*this); }

        SuspendAlways initial_suspend() { return {}; }

        SuspendAlways final_suspend() noexcept { return {}; }

        auto unhandled_exception() { std::terminate(); }
    };

    Resumable(promise_type::coro_handle handle) : handle_(handle) {}
    Resumable(const Resumable&) = delete;
    Resumable(Resumable&& o) noexcept : handle_(o.handle_) {
        o.handle_ = nullptr;
    }

    ~Resumable() {
        if (handle_) handle_.destroy();
    }

    std::string GetCurrentMessage() const {
        return messages[handle_.promise().current_value];
    }

    bool resume() {
        std::cout << "[Resumable::resume] call with current_value="
                  << handle_.promise().current_value << std::endl;

        if (!handle_.done()) {
            std::cout << "[Resumable::resume] prepare to start promise_type::resume" << std::endl;
            handle_.resume();
        }
        return !handle_.done();
    }

private:
    promise_type::coro_handle handle_;
};

Resumable foo() {
    size_t num{};
    for (;;) {
        std::cout << "[coro] start iteration: num=" << num << std::endl;
        co_yield num++;

        if (num == messages.size()) {
            break;
        }
    }
}

int main() {
    auto c = foo();
    std::cout << "[main] c created" << std::endl;

    while (c.resume()) {
        std::cout << "[main] start iteration" << std::endl;
        std::cout << c.GetCurrentMessage() << std::endl;
    }
}