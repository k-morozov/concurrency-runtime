//
// Created by konstantin on 07.06.24.
//

#include "async_mutex.h"

#include "awaiter/suspend.h"

namespace fibers {

void AsyncMutex::Lock() {
    Waiter::Guard guard(spinlock_);
    if (locked_) {
        Waiter waiter(this, std::move(guard));
        Suspend(&waiter);
    } else {
        locked_ = true;
    }
}
void AsyncMutex::Unlock() {
    Waiter* next_waiter = nullptr;
    {
        Waiter::Guard guard(spinlock_);
        if (waiters_.IsEmpty()) {
            locked_ = false;
            return;
        }
        next_waiter = waiters_.Pop();
    }

    if (next_waiter) {
        next_waiter->Schedule();
    }
}
void AsyncMutex::Park(Waiter* waiter) {
    waiters_.Push(waiter);
}

}  // namespace fibers