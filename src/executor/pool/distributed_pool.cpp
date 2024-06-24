//
// Created by konstantin on 24.06.24.
//

#include "distributed_pool.h"

namespace NExecutors {

DistributedPool::DistributedPool(const size_t count_)
    : IExecutor(), count_workers(count_) {
    for(size_t i{}; i<count_workers; i++) {
        workers.emplace_back();
    }
}

DistributedPool::~DistributedPool() {
    for(auto& w : workers) {
        w.Join();
    }
}

void DistributedPool::Start() {
    for(auto& w : workers) {
        w.Start();
    }
}

void DistributedPool::Submit(NExecutors::TaskBase *) {}


}  // namespace NExecutors