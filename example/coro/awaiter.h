//
// Created by konstantin on 17.07.24.
//

#pragma once

#include <coroutine>

struct Awaiter;
struct evt_awaiter_t;
using coro_t = std::coroutine_handle<>;

struct Awaiter {
    evt_awaiter_t& event_;
    coro_t coro_ = nullptr;

    Awaiter(evt_awaiter_t& event);

    bool await_ready() const;
    void await_suspend(coro_t coro) noexcept;
    void await_resume() const noexcept;
};

