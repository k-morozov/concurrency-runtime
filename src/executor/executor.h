//
// Created by konstantin on 04.06.24.
//

#pragma once

#include <executor/task/task.h>
#include <executor/task/task_base.h>

namespace NExecutors {

struct IExecutor {
    virtual ~IExecutor() noexcept = default;
    virtual void Submit(NExecutors::TaskBase* /*task*/, bool is_internal = false) = 0;

    std::atomic<size_t> count_tasks{0};
    std::atomic<bool> shutdown_{false};

    std::mutex mutex;
    std::condition_variable empty_tasks_;
};

}  // namespace executors
