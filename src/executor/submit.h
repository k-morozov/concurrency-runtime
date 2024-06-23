//
// Created by konstantin on 05.06.24.
//

#pragma once

#include <coro/routine.h>
#include <executor/executor.h>

namespace NExecutors {

void Submit(IExecutor& executor, fibers::coro::Routine routine);

}  // namespace NExecutors
