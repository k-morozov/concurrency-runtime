//
// Created by konstantin on 22.06.24.
//

#pragma once

#include <executor/task/task_base.h>
#include <components/function/basic_function.h>
#include <coro/routine.h>

namespace NExecutors {

class TaskFreeFunction : public TaskBase {
    fibers::coro::Routine routine;

public:
    void Run() noexcept override;

    static TaskBase* Make(fibers::coro::Routine routine_);

private:
    explicit TaskFreeFunction(fibers::coro::Routine routine_);
};

}  // namespace NExecutors
