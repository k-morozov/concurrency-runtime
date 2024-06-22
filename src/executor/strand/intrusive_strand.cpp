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
    NExecutors::Submit(underlying, [this]() {
        TaskList scheduled_tasks;
        {
            std::lock_guard lock(spinlock);
            std::swap(scheduled_tasks, tasks);
            tasks = {};
        }

        assert(!scheduled_tasks.IsEmpty());

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
    });
}

}  // namespace NExecutors