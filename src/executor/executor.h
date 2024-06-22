//
// Created by konstantin on 04.06.24.
//

#pragma once

#include <executor/task/task.h>
#include <executor/task/task_base.h>

namespace NExecutors {

struct IExecutor {
    virtual ~IExecutor() noexcept = default;
    virtual void Submit(NExecutors::TaskBase* /*task*/) = 0;
};

}  // namespace executors
