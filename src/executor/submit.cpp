//
// Created by konstantin on 05.06.24.
//

#include "submit.h"

namespace executors {

void Submit(IExecutor& executor, TaskPtr task) {
    executor.Submit(std::move(task));
}

}  // namespace executors