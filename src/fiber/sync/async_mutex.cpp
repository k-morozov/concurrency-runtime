//
// Created by konstantin on 07.06.24.
//

#include "async_mutex.h"

namespace fibers {

void AsyncMutex::Lock() {
    Guard guard;
    if (locked_) {
        AsyncMutexWaiter waiter(this, std::move(guard));
        // suspend waiter
    } else {
        locked_ = true;
    }
}
void AsyncMutex::Unlock() {
    AsyncMutexWaiter* next_waiter = nullptr;
    {
        // guard
        // waiter empty -> lock false
        // next == front
    }

    if (nullptr != next_waiter) {
//        next_waiter->Schedule();
    }
}
void AsyncMutex::Park(AsyncMutex::AsyncMutexWaiter* waiter) {}

void AsyncMutex::AsyncMutexWaiter::AwaitSuspend(FiberHandle current_fiber) {}

}  // namespace fibers