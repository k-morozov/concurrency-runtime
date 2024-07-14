//
// Created by konstantin on 13.07.24.
//

#include <coroutine>
#include <iostream>

template <typename T>
struct Generator {
    struct promise_type {
        using coro_handle = std::coroutine_handle<promise_type>;

        T current_value;
        auto yield_value(T value) {
            current_value = value;
            return std::suspend_always();
        }

        std::suspend_always initial_suspend() { return {}; }

        std::suspend_always final_suspend() noexcept { return {}; }

        auto get_return_object() { return coro_handle ::from_promise(*this); }

        auto unhandled_exception() { std::terminate(); }
    };

    Generator(promise_type::coro_handle handle) : handle_(handle) {}

    ~Generator() noexcept {
        if (handle_) handle_.destroy();
    }

    bool resume() {
        if (!handle_.done()) handle_();
        return !handle_.done();
    }

    T current_value() const { return handle_.promise().current_value; }

    bool move_next() const {
        return (handle_ ? (handle_.resume(), !handle_.done()) : false);
    }

private:
    promise_type::coro_handle handle_;
};

Generator<int> natural_nums() {
    int num = 0;
    for (;;) {
        co_yield num;
        num++;
    }
}

int main() {
    auto nums = natural_nums();
    for (size_t i{}; i < 5; i++) {
        nums.move_next();
        std::cout << nums.current_value() << std::endl;
    }
}