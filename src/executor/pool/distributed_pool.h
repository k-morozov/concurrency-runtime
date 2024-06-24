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
    std::deque<internal::Worker> workers;
    //    global_tasks
public:
    explicit DistributedPool(size_t count);
    ~DistributedPool() override;

    void Start();
    void Submit(NExecutors::TaskBase* /*task*/) override;
};

}  // namespace NExecutors