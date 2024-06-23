//
// Created by konstantin on 07.06.24.
//

#pragma once

#include <mutex>

#include <fiber/awaiter/mutex_awaiter.h>
#include <components/intrusive/list.h>

#include <components/sync/spinLock.h>

namespace fibers {

class AsyncMutex {
    using Spinlock = NSync::SpinLock;
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
    NComponents::IntrusiveList<Waiter> waiters_;

    void Park(Waiter* waiter);
};

}  // namespace fibers
