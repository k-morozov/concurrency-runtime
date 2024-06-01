//
// Created by konstantin on 29.05.24.
//

#include "fiber.h"

#include <cassert>
#include <memory>

#include <executor/task/fiber_task.h>
#include <executor/thread_pool/thread_pool.h>

thread_local fibers::Fiber* current_fiber;

namespace fibers {

Fiber::Fiber(pool::ThreadPool* executor, coro::Routine routine,
             ctx::Buffer&& buffer)
    : executor_(executor), fiber_coro_(std::move(routine), std::move(buffer)) {}

void Fiber::Schedule() {
    executor_->Submit(std::make_shared<pool::FiberTask>(([this]() { Run(); })));
}

void Fiber::Run() {
    current_fiber = this;

    fiber_coro_.Resume();
    if (fiber_coro_.IsCompleted()) {
        delete this;
        return;
    }

    current_fiber = nullptr;
    Schedule();
}
Fiber* Fiber::Self() { return current_fiber; }

void Fiber::Yield() {
    assert(current_fiber);
    Self()->fiber_coro_.Suspend();
}

pool::ThreadPool* Fiber::GetScheduler() { return executor_; }

}  // namespace fibers
