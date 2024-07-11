//
// Created by konstantin on 22.06.24.
//

#include "task_free_function.h"

namespace NExecutors {

TaskFreeFunction::TaskFreeFunction(NFibers::NCoro::Routine routine_)
    : routine(std::move(routine_)) {}

void TaskFreeFunction::Run() noexcept {
    routine();

    delete this;
}

TaskBase* TaskFreeFunction::Make(NFibers::NCoro::Routine routine_) {
    return new TaskFreeFunction(std::move(routine_));
}

}  // namespace NExecutors