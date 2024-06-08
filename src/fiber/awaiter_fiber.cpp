//
// Created by konstantin on 06.06.24.
//

#include "awaiter_fiber.h"

#include <cassert>

#include <executor/task/fiber_task.h>
#include <fiber/awaiter/awaiter.h>
#include <fiber/handle/fiber_handle.h>

namespace {

thread_local fibers::AwaiterFiber* current_fiber;

}

namespace fibers {

AwaiterFiber::AwaiterFiber(executors::IExecutor* executor,
                           coro::Routine routine, ctx::Buffer&& buffer)
    : executor_(executor), fiber_coro_(std::move(routine), std::move(buffer)) {}

void AwaiterFiber::Schedule() {
    executor_->Submit(
        std::make_shared<executors::FiberTask>(([this]() { Run(); })));
}

void AwaiterFiber::Run() {
    Switch();

    if (fiber_coro_.IsCompleted()) {
        delete this;
        return;
    }

    if (awaiter_) {
        awaiter_->AwaitSuspend(FiberHandle{this});
    }
}

AwaiterFiber* AwaiterFiber::Self() { return current_fiber; }

executors::IExecutor* AwaiterFiber::GetScheduler() { return executor_; }

void AwaiterFiber::Suspend(IAwaiter* waiter) {
    assert(waiter);
    awaiter_ = waiter;
    fiber_coro_.Suspend();
}

void AwaiterFiber::Switch() {
    current_fiber = this;
    fiber_coro_.Resume();
    current_fiber = nullptr;
}

}  // namespace fibers