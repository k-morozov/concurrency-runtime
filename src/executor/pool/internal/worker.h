//
// Created by konstantin on 24.06.24.
//

#pragma once

#include <optional>
#include <thread>

#include <executor/task/task_base.h>

namespace NExecutors::internal {

class Worker final {
    std::optional<std::thread> thread;
    // local_tasks
public:
    Worker();

    void Start();
    void Join();

    void Push(TaskBase*/*, SchedulerHint*/);
};

}  // namespace NExecutors::internal