//
// Created by konstantin on 17.07.24.
//

#pragma once

#include <list>

#include "awaiter.h"

struct Awaiter;

struct evt_awaiter_t {
    struct awaiter;
    std::list<Awaiter> awaiters;
    bool is_set_{};

    void set();

    bool is_set() const { return is_set_; }
    void reset() { is_set_ = false; }

    void push_awaiter(Awaiter waiter);

    Awaiter operator co_await () noexcept;
};

inline evt_awaiter_t g_evt;