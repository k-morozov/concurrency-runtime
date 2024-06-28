//
// Created by konstantin on 09.06.24.
//

#include "wait_group.h"

#include <fiber/awaiter/wait_group_awaiter.h>
#include <fiber/awaiter/suspend.h>

namespace fibers {

void WaitGroup::Add(const size_t add) {
    Waiter::Guard guard(spinlock_);
    counter_ += add;
}

void WaitGroup::Done() {
    Waiter::Guard guard(spinlock_);
    counter_ -= 1;
    if (0 == counter_) {
        bool no_task = wg_waiters_.IsEmpty();
        guard.unlock();

        if (no_task)
            return;
        do {
            guard.lock();

            auto wg_waiter = wg_waiters_.Pop();
            no_task = wg_waiters_.IsEmpty();
            guard.unlock();

            wg_waiter->Schedule();
        } while (!no_task);
    }
}

void WaitGroup::Wait() {
    Waiter::Guard guard(spinlock_);
    if (counter_ > 0) {
        Waiter wg_waiter(this, std::move(guard));
        Suspend(&wg_waiter);
    }
}

void WaitGroup::Park(WaitGroup::Waiter *waiter) {
    wg_waiters_.Push(waiter);
}

}  // namespace fibers
