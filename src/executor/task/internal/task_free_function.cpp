//
// Created by konstantin on 22.06.24.
//

#include "task_free_function.h"

namespace NExecutors {

TaskFreeFunction::TaskFreeFunction(fibers::coro::Routine routine_)
    : routine(std::move(routine_)) {}

void TaskFreeFunction::Run() noexcept {
    routine();

    delete this;
}

TaskBase* TaskFreeFunction::Make(fibers::coro::Routine routine_) {
    return new TaskFreeFunction(std::move(routine_));
}

}  // namespace NExecutors