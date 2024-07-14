//
// Created by konstantin on 13.07.24.
//

#include <coroutine>
#include <iostream>

struct Resumable {
    struct promise_type {
        using coro_handle = std::coroutine_handle<promise_type>;

        auto get_return_object() { return coro_handle ::from_promise(*this); }

        auto initial_suspend() { return std::suspend_always(); }

        auto final_suspend() noexcept { return std::suspend_always(); }

        void return_void() {}

        auto unhandled_exception() { std::terminate(); }
    };

    Resumable(promise_type::coro_handle handle) : handle_(handle) {}
    Resumable(const Resumable&) = delete;
    Resumable(Resumable&& o) noexcept : handle_(o.handle_) {
        o.handle_ = nullptr;
    }

    ~Resumable() {
        if (handle_)
            handle_.destroy();
    }

    bool resume() {
        if (!handle_.done()) {
            handle_.resume();
        }
        return !handle_.done();
    }

private:
    promise_type::coro_handle handle_;
};

Resumable foo() {
    std::cout << "Hello";
    co_await std::suspend_always();
    std::cout << " world!";
}

int main() {
    auto c = foo();
    c.resume();
    c.resume();
    c.resume();
}