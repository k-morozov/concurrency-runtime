//
// Created by konstantin on 17.06.24.
//

#include "intrusive_strand.h"

#include <memory>

#include <executor/task/internal/task_free_function.h>

namespace executors {

IntrusiveStrand::IntrusiveStrand(IExecutor& underlying_) : underlying(underlying_) {}

void IntrusiveStrand::Submit(TaskPtr /*task_*/) {
    throw std::runtime_error("IntrusiveStrand doesn't implemented simple task");
}

void IntrusiveStrand::Submit(NExecutors::TaskBase* task) {
    {
        std::lock_guard lock(spinlock);
        tasks.Push(task);
    }

    bool not_scheduled = false;
    if (is_schedule.compare_exchange_strong(not_scheduled, true)) {
        SubmitInternal();
    }
}

void IntrusiveStrand::SubmitInternal() {
    underlying.Submit(NExecutors::TaskFreeFunction::Make([this]() {
        intrusive::List<NExecutors::TaskBase> scheduled_tasks;
        {
            std::lock_guard lock(spinlock);
            std::swap(scheduled_tasks, tasks);
        }
        while (!scheduled_tasks.IsEmpty()) {
            auto task = scheduled_tasks.Pop();
            task->Run();
        }

        std::lock_guard lock(spinlock);
        if (tasks.IsEmpty()) {
            is_schedule.store(false);
            return;
        }
        SubmitInternal();
    }));
}

}  // namespace executors