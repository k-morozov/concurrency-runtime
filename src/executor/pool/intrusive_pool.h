//
// Created by konstantin on 19.06.24.
//

#pragma once

#include <atomic>
#include <memory>
#include <thread>
#include <vector>

#include <executor/executor.h>
#include <executor/task/task.h>
#include <executor/task/task_base.h>
#include <components/sync/spinLock.h>

namespace NExecutors {

class IntrusiveThreadPool final : public IExecutor {
public:
    explicit IntrusiveThreadPool(size_t count);
    ~IntrusiveThreadPool() override;

    void Start();

    void Submit(TaskBase* task, bool is_internal) override;

    static IExecutor* Current();

    void StartShutdown();

    void WaitShutdown();

    void WaitIdle();

private:
    const size_t workers_count_;

    std::vector<std::unique_ptr<std::thread>> workers_;
    std::mutex mutex;
    NComponents::IntrusiveList<TaskBase> tasks;

    std::atomic<bool> shutdown_{false};

    size_t count_workers_{0};
    std::mutex mutex_workers_;
    std::condition_variable empty_workers_;

    std::atomic<size_t> count_tasks_{0};
    std::mutex mutex_tasks_;
    std::condition_variable empty_tasks_;
};

}  // namespace executors
