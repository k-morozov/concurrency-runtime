//
// Created by konstantin on 22.06.24.
//

#include "intrusive_manual_executor.h"

namespace NExecutors {

void IntrusiveManualExecutor::Submit(TaskBase* task_) {
    std::lock_guard lock(spinlock);
    tasks.Push(task_);
    count_tasks++;
}

size_t IntrusiveManualExecutor::RunAtMost(const size_t limit) {
    size_t done_tasks = 0;
    while (done_tasks != limit) {
        TaskBase* task{nullptr};
        {
            std::lock_guard lock(spinlock);
            if (count_tasks == 0) {
                break;
            }
            task = tasks.Pop();
            count_tasks--;
        }
        task->Run();
        done_tasks++;
    }
    return done_tasks;
}

size_t IntrusiveManualExecutor::Drain() {
    size_t done_tasks = 0;
    while (true) {
        TaskBase* task{nullptr};
        {
            std::lock_guard lock(spinlock);
            if (count_tasks == 0) {
                break;
            }
            task = tasks.Pop();
            count_tasks--;
        }
        task->Run();
        done_tasks++;
    }
    return done_tasks;
}

size_t IntrusiveManualExecutor::TaskCount() const {
    std::lock_guard lock(spinlock);
    return count_tasks;
}

bool IntrusiveManualExecutor::IsEmpty() const {
    std::lock_guard lock(spinlock);
    return count_tasks == 0;
}

bool IntrusiveManualExecutor::NonEmpty() const { return !IsEmpty(); }

}  // namespace NExecutors