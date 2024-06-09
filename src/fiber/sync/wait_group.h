//
// Created by konstantin on 09.06.24.
//

#pragma once

#include <list>
#include <mutex>

#include <fiber/awaiter/mutex_awaiter.h>
#include <fiber/intrusive/list.h>
#include <fiber/sync/spinLock.h>
#include <fiber/awaiter/wait_group_awaiter.h>

namespace fibers {

class WaitGroup {
    using Spinlock = SpinLock;
    using Waiter = WaitGroupWaiter<WaitGroup>;

    friend Waiter;

public:
    void Add(size_t);
    void Done();
    void Wait();

private:
    size_t counter_{0};
    Spinlock spinlock_;

    intrusive::List<Waiter> wg_waiters_;

    void Park(Waiter* waiter);
};

}  // namespace fibers
