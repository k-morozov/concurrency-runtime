//
// Created by konstantin on 07.06.24.
//

#include "async_mutex.h"

#include <fiber/suspend.h>

namespace fibers {

void AsyncMutex::Lock() {
    Guard guard(spinlock_);
    if (locked_) {
        auto* waiter = new AsyncMutexWaiter(this, std::move(guard));
        Suspend(waiter);
    } else {
        locked_ = true;
    }
}
void AsyncMutex::Unlock() {
    AsyncMutexWaiter* next_waiter = nullptr;
    {
        Guard guard(spinlock_);
        if (waiters_.empty()) {
            locked_ = false;
            return;
        }
        next_waiter = waiters_.front();
        waiters_.pop_front();
    }

    if (next_waiter) {
        next_waiter->Schedule();
        delete next_waiter;
    }
}
void AsyncMutex::Park(AsyncMutex::AsyncMutexWaiter* waiter) {
    waiters_.push_back(waiter);
}

void AsyncMutex::AsyncMutexWaiter::AwaitSuspend(FiberHandle handle) {
    stopped_handle = handle;
    mutex->Park(this);
    guard.unlock();
}

void AsyncMutex::AsyncMutexWaiter::Schedule() {
    stopped_handle.Schedule();
}

}  // namespace fibers