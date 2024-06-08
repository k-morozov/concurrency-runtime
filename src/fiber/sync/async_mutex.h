//
// Created by konstantin on 07.06.24.
//

#pragma once

#include <list>
#include <mutex>

#include <fiber/awaiter/mutex_awaiter.h>
#include <fiber/sync/spinLock.h>
#include <fiber/intrusive/list.h>

namespace fibers {

class AsyncMutex {
    using Spinlock = SpinLock;
    using Waiter = AsyncMutexWaiter<AsyncMutex>;

    friend Waiter;

public:
    void lock() { Lock(); };
    void unlock() { Unlock(); };

    void Lock();
    void Unlock();

private:
    Spinlock spinlock_;
    bool locked_{false};
    intrusive::List<Waiter> waiters_;

    void Park(Waiter* waiter);
};

}  // namespace fibers
