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
    assert(0 == GetTasks());
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
                TaskBase* task{};
                {
                    std::lock_guard lock(mutex);
                    task = tasks.Pop();
                }

                if (task) {
                    task->Run();
                    RemoveTask();
                    NotifyAll();
                } else {
                    if (IsShutdown()) {
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

void IntrusiveThreadPool::Submit(TaskBase* task) {
    if (IsShutdown()) {
        return;
    }
    AddTask();

    std::lock_guard lock(mutex);
    tasks.Push(task);
}

IExecutor* IntrusiveThreadPool::Current() { return CurrentPool; }

void IntrusiveThreadPool::WaitShutdown() {
    std::unique_lock lock(mutex_workers_);
    while (0 != count_workers_) {
        empty_workers_.wait(lock);
    }
}

}  // namespace NExecutors