//
// Created by konstantin on 17.07.24.
//

#include "evt_awaiter_t.h"

#include <iostream>

void evt_awaiter_t::set() {
    is_set_ = true;
    auto temp = std::move(awaiters);
    std::cout << "evt_awaiter_t::set use " << temp.size() << " waiters" << std::endl;

    awaiters.clear();
    for (auto w : temp) {
        w.coro_.resume();
    }
}

void evt_awaiter_t::push_awaiter(Awaiter waiter) { awaiters.push_back(waiter); }

Awaiter evt_awaiter_t::operator co_await () noexcept {
    return Awaiter{*this};
}