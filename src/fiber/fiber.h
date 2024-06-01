//
// Created by konstantin on 29.05.24.
//

#pragma once

#include <context/buffer/buffer.h>
#include <coro/fiber_coro.h>
#include <pool/executor/executor.h>

namespace fibers::impl {

class Fiber {
public:
    Fiber(pool::Executor* executor, coro::Routine routine,
          ctx::Buffer&& buffer);

    void Schedule();

    void Run();

    static Fiber* Self();

    static void Yield();

    pool::Executor* GetScheduler();

private:
    pool::Executor* executor_;
    coro::FiberCoroutine fiber_coro_;
};

}  // namespace fibers::impl