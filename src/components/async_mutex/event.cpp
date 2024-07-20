//
// Created by konstantin on 19.07.24.
//
#include "event.h"

#include <iostream>
#include <syncstream>

namespace NComponents {

MutexAwaiter Event::operator co_await() {
    std::lock_guard lock(spinlock);
    return MutexAwaiter{*this};
}

void Event::ParkAwaiter(MutexAwaiter awaiter) {
    std::lock_guard lock(spinlock);
    std::osyncstream(std::cout) << "[Event::ParkAwaiter] add waiter, new size=" << waiters.size() + 1 << std::endl;
    waiters.push_back(awaiter);
}

}  // namespace NComponents