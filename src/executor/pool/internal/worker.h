//
// Created by konstantin on 24.06.24.
//

#pragma once

#include <mutex>
#include <optional>
#include <thread>

#include <components/intrusive/list.h>
#include <components/lock_free/simple_ms_queue.h>
#include <coro/stackfull_coro.h>
#include <executor/executor.h>
#include <executor/task/task_base.h>

namespace NExecutors::NInternal {

class Worker final {
    IExecutor* ex;
    std::optional<std::thread> thread;
    NComponents::SimpleMSQueue<TaskBase*> local_tasks;

    std::atomic<bool> suspended{};
    NFibers::NCoro::StackfullCoroutine coro;

    void Process();

public:
    explicit Worker(IExecutor* ex);
    ~Worker();

    void Start();
    void Join();

    void Push(TaskBase* /*, SchedulerHint*/);

    static IExecutor* Current();
};

}  // namespace NExecutors::NInternal