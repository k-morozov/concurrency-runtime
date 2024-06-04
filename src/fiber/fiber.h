//
// Created by konstantin on 29.05.24.
//

#pragma once

#include <context/buffer/buffer.h>
#include <coro/stackfull_coro.h>
#include <executor/executor.h>

namespace fibers {

class Fiber {
public:
    Fiber(pool::IExecutor* executor, coro::Routine routine,
          ctx::Buffer&& buffer);

    void Schedule();

    void Run();

    static Fiber* Self();

    static void Yield();

    pool::IExecutor* GetScheduler();

private:
    pool::IExecutor* executor_;
    coro::StackfullCoroutine fiber_coro_;
};

}  // namespace fibers