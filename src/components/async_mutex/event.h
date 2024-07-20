//
// Created by konstantin on 19.07.24.
//

#pragma once

#include <atomic>
#include <list>

#include <components/async_mutex/mutex_awaiter.h>

namespace NComponents {
struct Event final {
    std::atomic<bool> flag{};
    std::list<MutexAwaiter> waiters;

    Event() = default;

    bool TrySet() {
        return flag.exchange(true);
    }
    void UnSet() {
        flag.store(false);
        if (!waiters.empty()) {
            auto waiter = waiters.front();
            waiters.pop_front();
            waiter.coro.resume();
        }
    }

    bool IsSet() const { return flag.load(); }

    MutexAwaiter operator co_await();

    void ParkAwaiter(MutexAwaiter awaiter);
};
}  // namespace NComponents