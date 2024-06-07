//
// Created by konstantin on 07.06.24.
//

#pragma once

#include <mutex>

#include <fiber/awaiter.h>
#include <fiber/sync/spinLock.h>

namespace fibers {

class AsyncMutex {
    using Spinlock = SpinLock;
    using Guard = std::unique_lock<Spinlock>;

    class AsyncMutexWaiter : public IAwaiter {
    public:
        AsyncMutexWaiter(AsyncMutex* mutex, Guard guard)
            : mutex(mutex), guard(std::move(guard)) {};
        void AwaitSuspend(FiberHandle current_fiber) override;

        void Schedule();

    private:
        AsyncMutex* mutex;
        FiberHandle stopped_fiber;
        Guard guard;
    };

public:
    void lock() { Lock(); };
    void unlock() { Unlock(); };

    void Lock();
    void Unlock();

private:
    Spinlock spinlock_;
    bool locked_{false};
    // waiters

    void Park(AsyncMutexWaiter* waiter);
};

}  // namespace fibers
