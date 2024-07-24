//
// Created by konstantin on 19.07.24.
//
#include "event.h"

#include <iostream>
#include <syncstream>

namespace NComponents {

bool Event::TryLock() {
    //    std::unique_lock lock(spinlock);
    if (lock_flag) {
        std::osyncstream(std::cout)
            << "[Event::TryLock][thread_id=" << std::this_thread::get_id()
            << "] lock_flag was locked. Need park." << std::endl;
        return false;
    }

    std::osyncstream(std::cout)
        << "[Event::TryLock][thread_id=" << std::this_thread::get_id()
        << "] lock lock_flag." << std::endl;
    lock_flag = true;
    return true;
}

void Event::Unlock() {
//    assert(IsSet());
    std::unique_lock lock(spinlock);

    std::osyncstream(std::cout)
        << "[Event::Unlock][thread_id=" << std::this_thread::get_id()
        << "] call" << std::endl;

    if (!waiters.empty()) {
        std::cout << "[Event::Unlock] Waiters size=" << waiters.size()
                  << ", wake up first" << std::endl;
        auto waiter = std::move(waiters.front());
        waiters.pop_front();
        lock.unlock();

        waiter.Resume();
        return;
    }

    std::cout << "[Event::Unlock] Waiters empty. Set lock_flag to false"
              << std::endl;
    lock_flag = false;
}

MutexAwaiter Event::operator co_await() {
    std::unique_lock lock(spinlock);
    return MutexAwaiter{*this, std::move(lock)};
}

void Event::ParkAwaiter(MutexAwaiter&& awaiter) {
    //    std::lock_guard lock(spinlock);
    std::osyncstream(std::cout)
        << "[Event::ParkAwaiter][thead_id=" << std::this_thread::get_id()
        << "] add waiter, new size=" << waiters.size() + 1 << ", release lock"
        << std::endl;
    waiters.emplace_back(std::move(awaiter));
    waiters.back().ReleaseLock();
}

}  // namespace NComponents