//
// Created by konstantin on 09.05.24.
//

#pragma once

#include <atomic>
#include <memory>
#include <thread>
#include <vector>

#include <pool/task/task.h>
#include <pool_queue/blocking_queue/queue.h>

namespace pool {

class Executor;
std::shared_ptr<Executor> MakeThreadPool(size_t count);

class Executor final {
public:
    explicit Executor(size_t count);
    ~Executor();

    void StartShutdown();

    void WaitShutdown();

    void Submit(TaskPtr /*task*/);

private:
    std::vector<std::unique_ptr<std::thread>> workers_;
    UnboundedBlockingQueue<TaskPtr> queue_;

    std::atomic<bool> shutdown_{false};

    size_t count_workers_{0};
    std::mutex mutex_workers_;
    std::condition_variable empty_workers_;
};

}  // namespace pool