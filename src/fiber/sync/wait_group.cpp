//
// Created by konstantin on 09.06.24.
//

#include "wait_group.h"

#include <fiber/awaiter/suspend.h>
#include <fiber/awaiter/wait_group_awaiter.h>

namespace NFibers {

void WaitGroup::Add(const size_t add) {
    Waiter::Guard guard(spinlock_);
    counter_ += add;
}

void WaitGroup::Done() {
    Waiter* wg_waiter{};
    {
        Waiter::Guard guard(spinlock_);
        counter_ -= 1;
        if (0 == counter_) {
            wg_waiter = wg_waiters_.Pop();
            wg_waiters_.Clear();
        }
    }

    while (wg_waiter) {
        auto next = wg_waiter->Next();
        wg_waiter->Schedule();
        if (next == nullptr) {
            return;
        }
        wg_waiter = next->Cast();
    }
}

void WaitGroup::Wait() {
    Waiter::Guard guard(spinlock_);
    if (counter_ > 0) {
        Waiter wg_waiter(this, std::move(guard));
        Suspend(&wg_waiter);
    }
}

void WaitGroup::Park(WaitGroup::Waiter* waiter) { wg_waiters_.Push(waiter); }

}  // namespace NFibers
