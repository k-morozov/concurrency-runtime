//
// Created by konstantin on 05.06.24.
//

#pragma once

#include <coro/routine.h>
#include <executor/executor.h>
#include <executor/task/task.h>

namespace executors {

[[deprecated("use intrusive executor")]] void Submit(IExecutor& executor,
                                                     TaskPtr task);

void Submit(IExecutor& executor, fibers::coro::Routine routine);

}  // namespace executors
