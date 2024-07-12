//
// Created by konstantin on 19.06.24.
//
#include "intrusive_pool.h"

namespace NExecutors {

namespace {
thread_local IExecutor* CurrentPool;
}

IntrusiveThreadPool::IntrusiveThreadPool(size_t count)
    : IExecutor(), workers_count_(count){};

IntrusiveThreadPool::~IntrusiveThreadPool() {
    StartShutdown();
    WaitShutdown();

    for (auto& p : workers_) {
        if (p->joinable()) {
            p->join();
        }
    }
    assert(0 == count_tasks_.load());
}

void IntrusiveThreadPool::Start() {
    for (size_t i = 0; i < workers_count_; i++) {
        {
            std::lock_guard lock(mutex_workers_);
            count_workers_++;
        }

        workers_.push_back(std::make_unique<std::thread>([this] {
            CurrentPool = this;
            while (true) {
                TaskBase* task{nullptr};
                {
                    std::lock_guard lock(mutex);
                    task = tasks.Pop();
                }

                if (task) {
                    task->Run();
                    std::lock_guard lock_task(mutex_tasks_);
                    count_tasks_.fetch_sub(1);
                    empty_tasks_.notify_one();
                } else {
                    if (shutdown_.load()) {
                        break;
                    }
                }
            }

            {
                std::lock_guard lock(mutex_workers_);
                count_workers_--;
            }
            empty_workers_.notify_one();
        }));
    }
}

void IntrusiveThreadPool::Submit(TaskBase* task, const bool /*is_internal*/) {
    if (shutdown_.load()) {
        return;
    }
    count_tasks_.fetch_add(1);

    std::lock_guard lock(mutex);
    tasks.Push(task);
}

IExecutor* IntrusiveThreadPool::Current() { return CurrentPool; }

void IntrusiveThreadPool::StartShutdown() { shutdown_.store(true); }

void IntrusiveThreadPool::WaitShutdown() {
    std::unique_lock lock(mutex_workers_);
    while (0 != count_workers_) {
        empty_workers_.wait(lock);
    }
}

void IntrusiveThreadPool::WaitIdle() {
    std::unique_lock lock(mutex_tasks_);
    while (0 != count_tasks_.load()) {
        empty_tasks_.wait(lock);
    }
}

}  // namespace NExecutors