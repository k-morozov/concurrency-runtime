//
// Created by konstantin on 19.07.24.
//

#pragma once

#include <atomic>
#include <list>
#include <mutex>
#include <iostream>

#include <components/async_mutex/mutex_awaiter.h>
#include <components/sync/spinLock.h>

namespace NComponents {
struct Event final {
    mutable NSync::SpinLock spinlock;
    bool flag{};
    std::list<MutexAwaiter> waiters;

    Event() = default;

    bool TrySet() {
        std::lock_guard lock(spinlock);
        if (flag) {
            std::cout << "[Event::TrySet] flag was setted." << std::endl;
            return false;
        }

        std::cout << "[Event::TrySet] set flag" << std::endl;
        flag = true;
        return true;
    }

    void UnSet() {
        std::unique_lock lock(spinlock);
        std::cout << "[Event::TrySet] Unset flag" << std::endl;
        flag = false;
        if (!waiters.empty()) {
            std::cout << "[Event::TrySet] There is a waiter" << std::endl;
            auto waiter = waiters.front();
            waiters.pop_front();
            lock.unlock();

            waiter.coro.resume();
            return;
        }
        std::cout << "[Event::TrySet] No waiter" << std::endl;
    }

    bool IsSet() const {
        std::lock_guard lock(spinlock);
        return flag;
    }

    MutexAwaiter operator co_await();

    void ParkAwaiter(MutexAwaiter awaiter);
};
}  // namespace NComponents