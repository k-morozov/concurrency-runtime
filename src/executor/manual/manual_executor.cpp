//
// Created by konstantin on 04.06.24.
//

#include "manual_executor.h"

namespace executors {

void ManualExecutor::Submit(TaskPtr task) { tasks_.push(std::move(task)); }

size_t ManualExecutor::RunAtMost(const size_t limit) {
    size_t done_tasks = 0;
    while (NonEmpty() && done_tasks != limit) {
        auto task = std::move(tasks_.front());
        tasks_.pop();
        task->Run();
        done_tasks++;
    }
    return done_tasks;
}

size_t ManualExecutor::Drain() {
    size_t done_tasks = 0;
    while (NonEmpty()) {
        auto task = std::move(tasks_.front());
        tasks_.pop();
        task->Run();
        done_tasks++;
    }
    return done_tasks;
}

size_t ManualExecutor::TaskCount() const { return tasks_.size(); }

bool ManualExecutor::IsEmpty() const { return tasks_.empty(); }

bool ManualExecutor::NonEmpty() const { return !IsEmpty(); }

}  // namespace executors