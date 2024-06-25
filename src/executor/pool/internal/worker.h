//
// Created by konstantin on 24.06.24.
//

#pragma once

#include <optional>
#include <mutex>
#include <thread>

#include <executor/task/task_base.h>
#include <components/intrusive/list.h>
#include <executor/executor.h>

namespace NExecutors::internal {

class Worker final {
    IExecutor* ex;
    std::optional<std::thread> thread;

    std::mutex mutex;
    NComponents::IntrusiveList<TaskBase> local_tasks;

    std::atomic<bool> shutdown_worker{false};
    std::atomic<size_t> count_local_tasks{0};
public:
    explicit Worker(IExecutor* ex);
    ~Worker();

    void Start();
    void Join();

    void Push(TaskBase*/*, SchedulerHint*/);

    static IExecutor* Current();
};

}  // namespace NExecutors::internal