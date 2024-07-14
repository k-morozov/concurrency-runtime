//
// Created by konstantin on 24.06.24.
//

#include "distributed_pool.h"

#include <executor/pool/internal/worker.h>

namespace NExecutors {

DistributedPool::DistributedPool(const size_t count_)
    : IExecutor(), count_workers(count_) {
    for (size_t i{}; i < count_workers; i++) {
        workers.emplace_back(this);
    }
}

DistributedPool::~DistributedPool() { WaitShutdown(); }

void DistributedPool::Start() {
    for (auto& w : workers) {
        w.Start();
    }
}

void DistributedPool::Submit(NExecutors::TaskBase* task) {
    if (task->GetState() == StateTask::PLANNED) {
        AddTask();
        task->ProgressState();
    }

    if (auto worker = task->GetWorker()) {
        worker->Push(task);
    } else {
        global_tasks.Push(task);
    }
}

IExecutor* DistributedPool::Current() { return NInternal::Worker::Current(); }

void DistributedPool::WaitShutdown() {
    StartShutdown();
    for (auto& w : workers) {
        w.WakUpForShutdown();
        w.Join();
    }
}

}  // namespace NExecutors