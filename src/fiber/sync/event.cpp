//
// Created by konstantin on 10.06.24.
//

#include "event.h"

#include <fiber/awaiter/suspend.h>

namespace NFibers {

void Event::Wait() {
    Waiter::Guard guard(spinlock_);

    Waiter event_waiter{this, std::move(guard)};
    Suspend(&event_waiter);
}

void Event::Fire() {
    Waiter::Guard guard(spinlock_);
    while (!event_waiters_.IsEmpty()) {
        auto* event_waiter = event_waiters_.Pop();
        event_waiter->Schedule();
    }
}

void Event::Park(Waiter* waiter) { event_waiters_.Push(waiter); }

}  // namespace NFibers
