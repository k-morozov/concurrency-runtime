//
// Created by konstantin on 29.05.24.
//

#include "go.h"

#include <context/buffer/buffer.h>
#include <fiber/awaiter/suspend.h>
#include <fiber/awaiter/yield_awaiter.h>
#include <fiber/awaiter_fiber.h>

static const size_t kDefaultCoroBufferSize = 64 * 1024;

namespace NFibers {

void Go(NExecutors::IExecutor& scheduler, NCoro::Routine routine) {
    auto buffer = NContext::Buffer::AllocBytes(kDefaultCoroBufferSize);
    auto* fiber =
        new AwaiterFiber(&scheduler, std::move(routine), std::move(buffer));
    fiber->Schedule();
}

void Go(NCoro::Routine routine) {
    Go(*AwaiterFiber::Self()->GetScheduler(), std::move(routine));
}

void Yield() {
    YieldAwaiter waiter;
    Suspend(&waiter);
}

}  // namespace fibers