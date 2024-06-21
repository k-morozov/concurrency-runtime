//
// Created by konstantin on 04.06.24.
//

#pragma once

#include <executor/task/task.h>
#include <executor/task/task_base.h>

namespace executors {

struct IExecutor {
    virtual ~IExecutor() noexcept = default;
    virtual void Submit(TaskPtr /*task*/) = 0;
    virtual void Submit(NExecutors::TaskBase* /*task*/) {
        throw std::runtime_error("not implemented");
    };
};

}  // namespace executors
