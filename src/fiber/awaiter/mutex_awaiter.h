//
// Created by konstantin on 08.06.24.
//

#pragma once

#include <mutex>

#include <fiber/awaiter/awaiter.h>
#include <fiber/intrusive/list.h>

#include "sync/spinLock.h"

namespace fibers {

template <class M>
class AsyncMutexWaiter : public IAwaiter,
                         public intrusive::Node<AsyncMutexWaiter<M>> {
public:
    using Guard = std::unique_lock<typename M::Spinlock>;

    AsyncMutexWaiter(M* mutex, Guard guard)
        : mutex(mutex), guard(std::move(guard)){};

    void AwaitSuspend(StoppedFiber handle) override {
        assert(handle.IsValid());

        stopped_handle = handle;
        mutex->Park(this);
        guard.unlock();
    }

    void Schedule() { stopped_handle.Schedule(); }

private:
    M* mutex;
    StoppedFiber stopped_handle;
    Guard guard;
};

}  // namespace fibers
