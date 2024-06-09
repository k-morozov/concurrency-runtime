//
// Created by konstantin on 09.06.24.
//

#pragma once

#include <mutex>

#include <fiber/awaiter/awaiter.h>
#include <fiber/intrusive/list.h>

namespace fibers {

template <class W>
class WaitGroupWaiter : public IAwaiter,
                        public intrusive::Node<WaitGroupWaiter<W>> {
public:
    using Guard = std::unique_lock<typename W::Spinlock>;

    WaitGroupWaiter(W* wg, Guard guard) : wg(wg), guard(std::move(guard)){};

    void AwaitSuspend(StoppedFiber fiber) override {
        assert(fiber.IsValid());

        stopped_fiber = fiber;
        wg->Park(this);
        guard.unlock();
    }

    void Schedule() {
        stopped_fiber.Schedule();
    }

private:
    W* wg;
    Guard guard;
    StoppedFiber stopped_fiber;
};

}  // namespace fibers
