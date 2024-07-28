//
// Created by konstantin on 08.06.24.
//

#pragma once

#include <mutex>

#include <components/intrusive/list.h>
#include <components/sync/spinLock.h>
#include <components/sync/queue_spinlock.h>
#include <fiber/awaiter/awaiter.h>

namespace NFibers {

template <class M>
class AsyncMutexWaiter : public IAwaiter,
                         public NComponents::Node<AsyncMutexWaiter<M>> {
public:
    using Guard = NSync::QueueSpinLock::Guard;

    AsyncMutexWaiter(M* async_mutex, Guard& guard)
        : async_mutex(async_mutex), guard(guard){};

    void AwaitSuspend(StoppedFiber handle) override {
        assert(handle.IsValid());

        stopped_handle = handle;
        async_mutex->Park(this);

//        guard.release()->unlock();
        guard.Release();
    }

    void Schedule() { stopped_handle.Schedule(); }

private:
    M* async_mutex;
    StoppedFiber stopped_handle;
    Guard& guard;
};

}  // namespace NFibers
