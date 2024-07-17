//
// Created by konstantin on 17.07.24.
//

#include <coroutine>
#include <exception>
#include <iostream>

#include "evt_awaiter_t.h"

struct evt_awaiter_t;

struct resumable_no_own {
    struct promise_type {
        using coro_handle = std::coroutine_handle<promise_type>;

        auto get_return_object() { return coro_handle ::from_promise(*this); }
        std::suspend_never initial_suspend() const noexcept { return {}; }
        std::suspend_never final_suspend() const noexcept { return {}; }
        auto unhandled_exception() const noexcept { std::terminate(); }
    };

    resumable_no_own(promise_type::coro_handle) {}
};

resumable_no_own consumer1() {
    std::cout << "consumer1: step 1" << std::endl;
    co_await g_evt;
    std::cout << "consumer1: step 2" << std::endl;
}

resumable_no_own consumer2() {
    std::cout << "consumer2: step 1" << std::endl;
    co_await g_evt;
    std::cout << "consumer2: step 2" << std::endl;
    co_await g_evt;
    std::cout << "consumer2: step 3" << std::endl;
}

void producer() {
    std::cout << "producer: before set=" << g_evt.is_set() << std::endl;
    g_evt.set();
    std::cout << "producer: after set=" << g_evt.is_set() << std::endl;
}

int main() {
    consumer1();
    consumer2();
    producer();

    std::cout << "--------------------" << std::endl;

    consumer1();
    producer();
}