//
// Created by konstantin on 09.05.24.
//

#pragma once

#include <atomic>
#include <memory>
#include <thread>
#include <vector>

#include <executor/executor.h>
#include <executor/task/task.h>
#include <pool_queue/blocking_queue/queue.h>

namespace executors {

class ThreadPool;
std::shared_ptr<ThreadPool> MakeThreadPool(size_t count);

class ThreadPool final : public IExecutor {
public:
    explicit ThreadPool(size_t count);
    ~ThreadPool() override;

    void Start();

    void StartShutdown();

    void WaitShutdown();

    void WaitIdle();

    void Submit(TaskPtr /*task*/) override;

    static ThreadPool* Current();

private:
    const size_t workers_count_;

    std::vector<std::unique_ptr<std::thread>> workers_;
    UnboundedBlockingQueue<TaskPtr> queue_;

    std::atomic<bool> shutdown_{false};

    size_t count_workers_{0};
    std::mutex mutex_workers_;
    std::condition_variable empty_workers_;

    std::atomic<size_t> count_tasks_{0};
    std::mutex mutex_tasks_;
    std::condition_variable empty_tasks_;
};

}  // namespace executors