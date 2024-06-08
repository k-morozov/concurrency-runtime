//
// Created by konstantin on 29.05.24.
//

#include "go.h"

#include <context/buffer/buffer.h>
#include <fiber/awaiter.h>
#include <fiber/awaiter_fiber.h>

static const size_t kDefaultCoroBufferSize = 64 * 1024;

namespace fibers {

namespace {

class YieldAwaiter : public IAwaiter {
public:
    void AwaitSuspend(FiberHandle handle) override { handle.Schedule(); }
};

}  // namespace

void Go(executors::IExecutor& scheduler, coro::Routine routine) {
    auto buffer = ctx::Buffer::AllocBytes(kDefaultCoroBufferSize);
    auto* fiber =
        new AwaiterFiber(&scheduler, std::move(routine), std::move(buffer));
    fiber->Schedule();
}

void Go(coro::Routine routine) {
    Go(*AwaiterFiber::Self()->GetScheduler(), std::move(routine));
}

void Yield() {
    auto* waiter = new YieldAwaiter();
    AwaiterFiber::Self()->Suspend(waiter);
}

}  // namespace fibers