//
// Created by konstantin on 29.05.24.
//

#include "go.h"

#include <context/buffer/buffer.h>
#include <fiber/fiber.h>

static const size_t kDefaultCoroBufferSize = 64 * 1024;

namespace fibers {

void Go(pool::Executor& scheduler, go::impl::coro::Coroutine::Routine routine) {
    auto buffer = go::impl::ctx::Buffer::AllocBytes(kDefaultCoroBufferSize);
    auto* fiber = new go::impl::fiber::Fiber(&scheduler, std::move(routine),
                                             std::move(buffer));
    fiber->Schedule();
}

void Go(go::impl::coro::Coroutine::Routine routine) {
    Go(*go::impl::fiber::Fiber::Self()->GetScheduler(), std::move(routine));
}

void Yield() {
    go::impl::fiber::Fiber::Self()->Yield();
}

}  // namespace fibers