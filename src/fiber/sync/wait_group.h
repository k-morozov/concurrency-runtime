//
// Created by konstantin on 09.06.24.
//

#pragma once

#include <mutex>

#include <components/intrusive/list.h>
#include <components/sync/spinLock.h>
#include <components/sync/queue_spinlock.h>
#include <fiber/awaiter/wait_group_awaiter.h>

namespace NFibers {

class WaitGroup {
    using Spinlock = NSync::QueueSpinLock;
    using Waiter = WaitGroupWaiter<WaitGroup>;

    friend Waiter;

public:
    ~WaitGroup() { assert(wg_waiters_.IsEmpty()); }

    void Add(size_t);
    void Done();
    void Wait();

private:
    size_t counter_{0};
    Spinlock spinlock_;

    NComponents::IntrusiveList<Waiter> wg_waiters_;

    void Park(Waiter* waiter);
};

}  // namespace NFibers
