//
// Created by konstantin on 19.06.24.
//
#include "intrusive_pool.h"

namespace executors {

namespace {
thread_local executors::IExecutor* CurrentPool;
}

IntrusiveThreadPool::IntrusiveThreadPool(size_t count)
    : workers_count_(count){};

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
                NExecutors::TaskBase* task{nullptr};
                std::unique_lock lock(spinlock);

                task = tasks.Pop();
                lock.unlock();

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

void IntrusiveThreadPool::Submit(TaskPtr /*task*/) {
    throw std::runtime_error("not implemented");
};

void IntrusiveThreadPool::Submit(NExecutors::TaskBase* task) {
    if (shutdown_.load()) {
        return;
    }
    count_tasks_.fetch_add(1);

    std::lock_guard lock(spinlock);
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

}  // namespace executors