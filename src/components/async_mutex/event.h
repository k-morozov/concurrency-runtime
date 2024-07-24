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
    bool lock_flag{};
    std::list<MutexAwaiter> waiters;

    Event() = default;

    bool TryLock();

    void Unlock();

    bool IsSet() const {
        std::lock_guard lock(spinlock);
        return lock_flag;
    }

    MutexAwaiter operator co_await();

    void ParkAwaiter(MutexAwaiter&& awaiter);
};

}  // namespace NComponents