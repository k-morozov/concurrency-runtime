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
    std::mutex mutex;

    std::atomic<bool> shutdown_worker{false};
    std::atomic<size_t> count_local_tasks{0};
    std::condition_variable empty_tasks_;
public:
    explicit Worker(IExecutor* ex);
    ~Worker();

    void Start();
    void Join();

    void Push(TaskBase*/*, SchedulerHint*/);

    static IExecutor* Current();

private:
    void WaitIdle();
};

}  // namespace NExecutors::internal