//
// Created by konstantin on 10.06.24.
//

#pragma once

#include <mutex>

#include <fiber/awaiter/event_awaiter.h>
#include <fiber/intrusive/list.h>
#include <components/sync/spinLock.h>

namespace fibers {

class Event {
    using Spinlock = NSync::SpinLock;
    using Waiter = EventWaiter<Event>;

    friend Waiter;

public:
    void Wait();
    void Fire();

private:
    Spinlock spinlock_;
    intrusive::List<Waiter> event_waiters_;

    void Park(Waiter* waiter);
};
}  // namespace fibers
