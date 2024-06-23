//
// Created by konstantin on 05.06.24.
//

#include "submit.h"

#include <executor/task/internal/task_free_function.h>

namespace NExecutors {

void Submit(IExecutor& executor, fibers::coro::Routine routine) {
    executor.Submit(TaskFreeFunction::Make(std::move(routine)));
}

}  // namespace NExecutors