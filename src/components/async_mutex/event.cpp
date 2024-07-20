//
// Created by konstantin on 19.07.24.
//
#include "event.h"

#include <iostream>

namespace NComponents {

MutexAwaiter Event::operator co_await() {
    return MutexAwaiter{*this};
}

void Event::ParkAwaiter(MutexAwaiter awaiter) {
    std::lock_guard lock(spinlock);
    std::cout << "[Event::ParkAwaiter] add waiter, current size=" << waiters.size() << std::endl;
    waiters.push_back(awaiter);
}

}  // namespace NComponents