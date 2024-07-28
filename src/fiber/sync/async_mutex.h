//
// Created by konstantin on 07.06.24.
//

#pragma once

#include <mutex>

#include <components/intrusive/list.h>
#include <components/sync/spinLock.h>
#include <components/sync/queue_spinlock.h>
#include <fiber/awaiter/mutex_awaiter.h>

namespace NFibers {

class AsyncMutex {
    using Spinlock = NSync::QueueSpinLock;
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

}  // namespace NFibers
