//
// Created by konstantin on 19.07.24.
//

#pragma once

#include <atomic>
#include <list>
#include <mutex>
#include <iostream>
#include <syncstream>
#include <thread>

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
            std::osyncstream(std::cout) << "[Event::TrySet] flag was setted." << std::endl;
            return false;
        }

        std::osyncstream(std::cout) << "[Event::TrySet] set flag" << std::endl;
        flag = true;
        return true;
    }

    void UnSet() {
        std::unique_lock lock(spinlock);
        std::osyncstream(std::cout) << "[Event::UnSet][thread=" << std::this_thread::get_id() << "] call" << std::endl;

        flag = false;
        std::osyncstream(std::cout) << "[Event::UnSet][thread=" << std::this_thread::get_id() << "] unset flag" << std::endl;

        if (!waiters.empty()) {
            std::cout << "[Event::TrySet] Waiters size=" << waiters.size() << ", pop first" << std::endl;
            auto waiter = waiters.front();
            waiters.pop_front();
            lock.unlock();

            waiter.coro.resume();
            return;
        }
    }

    bool IsSet() const {
        std::lock_guard lock(spinlock);
        return flag;
    }

    MutexAwaiter operator co_await();

    void ParkAwaiter(MutexAwaiter awaiter);
};
}  // namespace NComponents