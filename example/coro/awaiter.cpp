//
// Created by konstantin on 17.07.24.
//

#include "awaiter.h"

#include <iostream>

#include "evt_awaiter_t.h"

Awaiter::Awaiter(evt_awaiter_t& event) : event_(event) {}

bool Awaiter::await_ready() const { return event_.is_set(); }

void Awaiter::await_suspend(coro_t coro) noexcept {
    coro_ = coro;
    event_.push_awaiter(*this);
}

void Awaiter::await_resume() const noexcept {
    std::cout << "Awaiter::await_resume call reset event" << std::endl;
    event_.reset();
}
