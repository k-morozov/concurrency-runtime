//
// Created by konstantin on 04.06.24.
//

#pragma once

#include <executor/task/task.h>

namespace executors {

struct IExecutor {
    virtual ~IExecutor() noexcept = default;
    virtual void Submit(TaskPtr /*task*/) = 0;
};

}  // namespace executors
