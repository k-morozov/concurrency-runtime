//
// Created by konstantin on 07.06.24.
//

#include "async_mutex.h"

#include "awaiter/suspend.h"

namespace fibers {

void AsyncMutex::Lock() {
    Waiter::Guard guard(spinlock_);
    if (locked_) {
        auto* waiter = new Waiter(this, std::move(guard));
        Suspend(waiter);
    } else {
        locked_ = true;
    }
}
void AsyncMutex::Unlock() {
    Waiter* next_waiter = nullptr;
    {
        Waiter::Guard guard(spinlock_);
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
void AsyncMutex::Park(Waiter* waiter) {
    waiters_.push_back(waiter);
}

}  // namespace fibers