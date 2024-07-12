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
    friend class internal::Worker;

    const size_t count_workers;
    std::atomic<size_t> current_worker{};
    std::deque<internal::Worker> workers;
    std::mutex mutex;

public:
    explicit DistributedPool(size_t count);
    ~DistributedPool() override;

    void Start();
    void Submit(NExecutors::TaskBase* /*task*/, bool is_internal) override;

    static IExecutor* Current();

    void StartShutdown();

    void WaitShutdown();

    void WaitIdle();
};

}  // namespace NExecutors