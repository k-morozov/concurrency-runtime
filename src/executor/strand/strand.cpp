//
// Created by konstantin on 17.06.24.
//

#include "strand.h"

#include <memory>

#include <task/fiber_task.h>

namespace executors {

Strand::Strand(IExecutor& underlying_) : underlying(underlying_) {}

void Strand::Submit(TaskPtr task_) {
    {
        std::lock_guard lock(spinlock);
        tasks.push_back(task_);
    }

    bool not_scheduled = false;
    if (is_schedule.compare_exchange_strong(not_scheduled, true)) {
        SubmitInternal();
    }
}

void Strand::SubmitInternal() {
    underlying.Submit(std::make_shared<executors::FiberTask>([this]() {
        TaskList scheduled_tasks;
        {
            std::lock_guard lock(spinlock);
            std::swap(scheduled_tasks, tasks);
        }
        while (!scheduled_tasks.empty()) {
            auto task = std::move(scheduled_tasks.front());
            scheduled_tasks.pop_front();
            task->Run();
        }

        std::lock_guard lock(spinlock);
        if (tasks.empty()) {
            is_schedule.store(false);
            return;
        }
        SubmitInternal();
    }));
}

}  // namespace executors