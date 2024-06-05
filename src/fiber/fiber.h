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
    Fiber(executors::IExecutor* executor, coro::Routine routine,
          ctx::Buffer&& buffer);

    void Schedule();

    void Run();

    static Fiber* Self();

    static void Yield();

    executors::IExecutor* GetScheduler();

private:
    executors::IExecutor* executor_;
    coro::StackfullCoroutine fiber_coro_;
};

}  // namespace fibers