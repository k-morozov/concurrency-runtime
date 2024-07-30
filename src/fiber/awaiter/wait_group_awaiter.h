//
// Created by konstantin on 09.06.24.
//

#pragma once

#include <mutex>

#include <components/intrusive/list.h>
#include <components/sync/queue_spinlock.h>
#include <fiber/awaiter/awaiter.h>

namespace NFibers {

template <class W>
class WaitGroupWaiter : public IAwaiter,
                        public NComponents::Node<WaitGroupWaiter<W>> {
public:
    using Guard = NSync::QueueSpinLock::Guard;

    WaitGroupWaiter(W* wg, Guard& guard) : wg(wg), guard(guard){};

    void AwaitSuspend(StoppedFiber fiber) override {
        assert(fiber.IsValid());

        stopped_fiber = fiber;
        wg->Park(this);
        guard.Release();
    }

    void Schedule() { stopped_fiber.Schedule(); }

private:
    W* wg;
    Guard& guard;
    StoppedFiber stopped_fiber;
};

}  // namespace NFibers
