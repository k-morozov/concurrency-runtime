//
// Created by konstantin on 22.06.24.
//

#pragma once

#include <executor/task/task_base.h>
#include <components/function/basic_function.h>
#include <coro/routine.h>

namespace NExecutors {

class TaskFreeFunction : public TaskBase {
    NFibers::NCoro::Routine routine;

public:
    TaskRunResult Run() noexcept override;

    static TaskBase* Make(NFibers::NCoro::Routine routine_);

private:
    explicit TaskFreeFunction(NFibers::NCoro::Routine routine_);
};

}  // namespace NExecutors
