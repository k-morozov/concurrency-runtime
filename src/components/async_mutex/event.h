//
// Created by konstantin on 19.07.24.
//

#pragma once

#include <atomic>
#include <cassert>
#include <iostream>
#include <list>
#include <mutex>
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
            std::osyncstream(std::cout)
                << "[Event::TrySet][thread_id=" << std::this_thread::get_id()
                << "] flag was locked. Need park." << std::endl;
            return false;
        }

        std::osyncstream(std::cout)
            << "[Event::TrySet][thread_id=" << std::this_thread::get_id()
            << "] lock flag." << std::endl;
        flag = true;
        return true;
    }

    void UnSet() {
        assert(IsSet());

        std::unique_lock lock(spinlock);
        std::osyncstream(std::cout)
            << "[Event::UnSet][thread_id=" << std::this_thread::get_id()
            << "] call, set status flag to false" << std::endl;

        if (!waiters.empty()) {
            std::cout << "[Event::TrySet] Waiters size=" << waiters.size()
                      << ", wake up first" << std::endl;
            auto waiter = waiters.front();
            waiters.pop_front();
            lock.unlock();

//            assert(waiter.coro.operator bool());
            waiter.coro.resume();
            return;
        }

        flag = false;
    }

    bool IsSet() const {
        std::lock_guard lock(spinlock);
        return flag;
    }

    MutexAwaiter operator co_await();

    void ParkAwaiter(MutexAwaiter awaiter);
};

}  // namespace NComponents