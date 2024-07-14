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

    std::optional<NFibers::NCoro::StackfullCoroutine> coro;

    std::atomic<std::size_t> counter_empty_tasks{};
    std::binary_semaphore smph{0};

    std::atomic<bool> shutdown{};

    void Process();
    void Loop();

public:
    explicit Worker(IExecutor* ex);
    ~Worker();

    void Start();
    void Join();

    void Push(TaskBase*);

    static IExecutor* Current();
};

}  // namespace NExecutors::NInternal