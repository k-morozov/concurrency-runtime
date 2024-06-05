//
// Created by konstantin on 05.06.24.
//

#pragma once

#include <executor/executor.h>
#include <executor/task/task.h>

namespace executors {

void Submit(IExecutor& executor, TaskPtr task);

}
