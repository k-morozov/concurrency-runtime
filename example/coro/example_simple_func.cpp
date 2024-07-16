//
// Created by konstantin on 16.07.24.
//

#include <coroutine>
#include <string>
#include <iostream>

struct resumable_no_wait final {
    struct hello_awaiter final {
        bool await_ready() const noexcept { return true; }
        void await_suspend(std::coroutine_handle<>) const noexcept {}
        void await_resume() const noexcept { std::cout << "Text example" << std::endl; }
    };

    struct promise_type {
        using coro_handle = std::coroutine_handle<promise_type>;

        auto get_return_object() { return coro_handle ::from_promise(*this); }

        std::suspend_never initial_suspend() { return {}; }

        std::suspend_never final_suspend() noexcept { return {}; }

        auto unhandled_exception() { std::abort(); }

        void return_void() {
            std::cout << "call return_void" << std::endl;
        }
    };

    resumable_no_wait(promise_type::coro_handle handle) : handle_(handle) {}
    resumable_no_wait(const resumable_no_wait&) = delete;
    resumable_no_wait(resumable_no_wait&& o) noexcept : handle_(o.handle_) {
        o.handle_ = nullptr;
    }

    ~resumable_no_wait() {
        std::cout << "start ~resumable_no_wait" << std::endl;
        if (handle_) {
            std::cout << "destroy" << std::endl;
//            handle_.destroy();
        }
        std::cout << "finish ~resumable_no_wait" << std::endl;
    }

private:
    promise_type::coro_handle handle_;
};

resumable_no_wait hello() {
    co_await resumable_no_wait::hello_awaiter{};
    std::cout << "Finish hello" << std::endl;
}

int main() {
    {
        auto h = hello();
        std::cout << "Finish h" << std::endl;

    }
    std::cout << "Finish main" << std::endl;
}