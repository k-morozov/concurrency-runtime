//
// Created by konstantin on 17.06.24.
//

#include "intrusive_strand.h"

#include <memory>

#include <executor/submit.h>

namespace NExecutors {

IntrusiveStrand::IntrusiveStrand(IExecutor& underlying_)
    : underlying(underlying_) {}

void IntrusiveStrand::Submit(TaskBase* task) {
    std::lock_guard lock(spinlock);
    tasks.Push(task);

    // @todo why plan should be with lock?
    PlanTask(State::READY);
}

void IntrusiveStrand::SubmitInternal() {
    NExecutors::Submit(underlying, [this]() { RunBatch(); });
}

void IntrusiveStrand::PlanTask(State from) {
    if (state.compare_exchange_strong(from, State::PLAN)) {
        SubmitInternal();
    }
}

bool IntrusiveStrand::ChangeState(State from, const State to) {
    return state.compare_exchange_strong(from, to);
}

void IntrusiveStrand::RunBatch() {
    {
        const bool result = ChangeState(State::PLAN, State::RUNNING);
        assert(result);
    }

    TaskList scheduled_tasks;
    {
        std::lock_guard lock(spinlock);
        assert(!tasks.IsEmpty());
        std::swap(scheduled_tasks, tasks);
    }

    while (!scheduled_tasks.IsEmpty()) {
        auto task = scheduled_tasks.Pop();
        task->Run();
    }

    if (IsTasksEmpty()) {
        const bool result = ChangeState(State::RUNNING, State::READY);
        assert(result);
    } else {
        PlanTask(State::RUNNING);
    }
}

}  // namespace NExecutors