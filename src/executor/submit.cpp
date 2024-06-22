//
// Created by konstantin on 05.06.24.
//

#include "submit.h"

#include <executor/task/internal/task_free_function.h>

namespace executors {

void Submit(IExecutor& executor, TaskPtr task) {
    executor.Submit(std::move(task));
}

void Submit(IExecutor& executor, fibers::coro::Routine routine) {
    executor.Submit(NExecutors::TaskFreeFunction::Make(std::move(routine)));
}

}  // namespace executors