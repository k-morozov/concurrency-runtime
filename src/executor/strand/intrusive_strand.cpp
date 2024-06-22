//
// Created by konstantin on 17.06.24.
//

#include "intrusive_strand.h"

#include <format>
#include <memory>

#include <executor/submit.h>

namespace NExecutors {

IntrusiveStrand::IntrusiveStrand(IExecutor& underlying_)
    : underlying(underlying_) {}

void IntrusiveStrand::Submit(TaskBase* task) {
    assert(task);
    {
        std::lock_guard lock(spinlock);
        tasks.Push(task);
    }

    PlanTaskFromReadyState();
}

void IntrusiveStrand::SubmitInternal() {
    NExecutors::Submit(underlying, [this]() {
        ChangeStateToRun();

        TaskList scheduled_tasks;
        {
            std::lock_guard lock(spinlock);
//            if (tasks.IsEmpty()) {
//                throw std::runtime_error(
//                    std::format("tasks is empty, state={}",
//                                static_cast<uint32_t>(state.load())));
//            }
            std::swap(scheduled_tasks, tasks);
            assert(tasks.IsEmpty());
        }

        while (!scheduled_tasks.IsEmpty()) {
            auto task = scheduled_tasks.Pop();
            task->Run();
        }

        std::lock_guard lock(spinlock);
        if (tasks.IsEmpty()) {
            ChangeStateToReady();
            return;
        }

        ChangeStateToPlan();
        SubmitInternal();
    });
}

void IntrusiveStrand::PlanTaskFromReadyState() {
    State ready = State::READY;
    if (state.compare_exchange_strong(ready, State::PLAN)) {
        SubmitInternal();
    }
}

void IntrusiveStrand::ChangeStateToRun() {
    State plan = State::PLAN;
    state.compare_exchange_strong(plan, State::RUNNING);
    assert(State::PLAN == plan);
}

void IntrusiveStrand::ChangeStateToReady() {
    State running = State::RUNNING;
    state.compare_exchange_strong(running, State::READY);
    assert(State::RUNNING == running);
}

void IntrusiveStrand::ChangeStateToPlan() {
    State running = State::RUNNING;
    state.compare_exchange_strong(running, State::PLAN);
    assert(State::RUNNING == running);
}

}  // namespace NExecutors