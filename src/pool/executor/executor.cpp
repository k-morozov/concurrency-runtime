//
// Created by konstantin on 09.05.24.
//

#include "executor.h"

#include <cassert>
#include <memory>
#include <thread>

thread_local pool::Executor* CurrentPool;

namespace pool {

std::shared_ptr<Executor> MakeThreadPool(const size_t count) {
    auto pool = std::make_shared<Executor>(count);
    pool->Start();
    return pool;
}

Executor::Executor(const size_t count) : workers_count_(count) {}

Executor::~Executor() {
    StartShutdown();
    WaitShutdown();

    for (auto& p : workers_) {
        if (p->joinable()) {
            p->join();
        }
    }
    assert(0 == count_tasks_.load());
}

void Executor::Start() {
    for (size_t i = 0; i < workers_count_; i++) {
        {
            std::lock_guard lock(mutex_workers_);
            count_workers_++;
        }

        workers_.push_back(std::make_unique<std::thread>([this] {
            CurrentPool = this;
            while (true) {
                auto task_opt = queue_.Take();
                if (!task_opt) {
                    break;
                }
                auto task_ptr = std::move(task_opt.value());
                assert(task_ptr);
                try {
                    if (!task_ptr->IsCanceled()) {
                        task_ptr->Run();
                        task_ptr->MarkCompleted();
                    }
                } catch (std::exception& ex) {
                    std::exception_ptr p = std::current_exception();
                    task_ptr->MarFailed(p);
                }

                std::lock_guard lock(mutex_tasks_);
                count_tasks_.fetch_sub(1);
                empty_tasks_.notify_one();
            }

            {
                std::lock_guard lock(mutex_workers_);
                count_workers_--;
            }
            empty_workers_.notify_one();
        }));
    }
}

void Executor::StartShutdown() {
    shutdown_.store(true);
    queue_.Close();
}

void Executor::WaitShutdown() {
    std::unique_lock lock(mutex_workers_);
    while (0 != count_workers_) {
        empty_workers_.wait(lock);
    }
}

void Executor::WaitIdle() {
    std::unique_lock lock(mutex_tasks_);
    while (0 != count_tasks_.load()) {
        empty_tasks_.wait(lock);
    }
}

void Executor::Submit(TaskPtr task) {
    if (shutdown_.load()) {
        return;
    }
    count_tasks_.fetch_add(1);
    [[maybe_unused]] bool status = queue_.Put(std::move(task));
}
Executor* Executor::Current() { return CurrentPool; }

}  // namespace pool
