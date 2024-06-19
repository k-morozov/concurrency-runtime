//
// Created by konstantin on 09.06.24.
//

#pragma once

#include <mutex>

#include <components/intrusive/list.h>
#include <fiber/awaiter/awaiter.h>

namespace fibers {

template <class E>
class EventWaiter : public IAwaiter, public intrusive::Node<EventWaiter<E>> {
public:
    using Guard = std::unique_lock<typename E::Spinlock>;

    EventWaiter(E* event, Guard guard)
        : event(event), guard(std::move(guard)){};

    void AwaitSuspend(StoppedFiber fiber) override {
        assert(fiber.IsValid());

        stopped_fiber = fiber;
        event->Park(this);
        guard.unlock();
    }

    void Schedule() { stopped_fiber.Schedule(); }

private:
    E* event;
    Guard guard;
    StoppedFiber stopped_fiber;
};

}  // namespace fibers
