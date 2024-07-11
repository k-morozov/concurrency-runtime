//
// Created by konstantin on 10.06.24.
//

#pragma once

#include <mutex>

#include <components/intrusive/list.h>
#include <components/sync/spinLock.h>
#include <fiber/awaiter/event_awaiter.h>

namespace NFibers {

class Event {
    using Spinlock = NSync::SpinLock;
    using Waiter = EventWaiter<Event>;

    friend Waiter;

public:
    void Wait();
    void Fire();

private:
    Spinlock spinlock_;
    NComponents::IntrusiveList<Waiter> event_waiters_;

    void Park(Waiter* waiter);
};
}  // namespace NFibers
