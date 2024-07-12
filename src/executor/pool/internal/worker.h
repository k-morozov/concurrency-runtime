//
// Created by konstantin on 24.06.24.
//

#pragma once

#include <mutex>
#include <optional>
#include <thread>

#include <components/intrusive/list.h>
#include <components/lock_free/simple_ms_queue.h>
#include <executor/executor.h>
#include <executor/task/task_base.h>

namespace NExecutors::internal {

class Worker final {
    IExecutor* ex;
    std::optional<std::thread> thread;
    NComponents::SimpleMSQueue<TaskBase*> local_tasks;

public:
    explicit Worker(IExecutor* ex);
    ~Worker();

    void Start();
    void Join();

    void Push(TaskBase* /*, SchedulerHint*/, bool is_internal);

    static IExecutor* Current();
};

}  // namespace NExecutors::internal