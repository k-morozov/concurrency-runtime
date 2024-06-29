//
// Created by konstantin on 24.06.24.
//

#pragma once

#include <deque>

#include <executor/executor.h>
#include <executor/pool/internal/worker.h>
#include <executor/task/task_base.h>

namespace NExecutors {

class DistributedPool final : public IExecutor {
    const size_t count_workers;
    std::atomic<size_t> current_worker{};

    std::deque<internal::Worker> workers;

    std::mutex mutex;
    NComponents::IntrusiveList<TaskBase> global_tasks;

    std::atomic<bool> shutdown_{false};
    std::atomic<size_t> count_tasks_{0};

public:
    explicit DistributedPool(size_t count);
    ~DistributedPool() override;

    void Start();
    void Submit(NExecutors::TaskBase* /*task*/) override;

    static IExecutor* Current();

    void StartShutdown();

    void WaitShutdown();

    void WaitIdle();
};

}  // namespace NExecutors