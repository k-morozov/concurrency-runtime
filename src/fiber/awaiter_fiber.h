//
// Created by konstantin on 06.06.24.
//

#pragma once

#include <context/buffer/buffer.h>
#include <coro/stackfull_coro.h>
#include <executor/executor.h>

namespace fibers {

class IAwaiter;

class AwaiterFiber final {
public:
    AwaiterFiber(executors::IExecutor* executor, coro::Routine routine,
                 ctx::Buffer&& buffer);

    void Schedule();

    void Suspend(IAwaiter* waiter);
    void Switch();

    void Run();

    static AwaiterFiber* Self();

    executors::IExecutor* GetScheduler();

private:
    executors::IExecutor* executor_;
    coro::StackfullCoroutine fiber_coro_;
    IAwaiter* awaiter_{nullptr};
};

}  // namespace fibers
