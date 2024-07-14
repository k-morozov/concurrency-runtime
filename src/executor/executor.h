//
// Created by konstantin on 04.06.24.
//

#pragma once

#include <components/lock_free/simple_ms_queue.h>
#include <executor/task/task.h>
#include <executor/task/task_base.h>

namespace NExecutors {

namespace NInternal {
class Worker;
}

class Shutdowner {
    std::atomic<size_t> count_tasks{0};
    std::atomic<bool> shutdown_{false};

    std::mutex mutex;
    std::condition_variable empty_tasks_;

public:
    virtual ~Shutdowner() noexcept = default;

    void WaitIdle() {
        std::unique_lock lock(mutex);
        while (0 != count_tasks.load()) {
            empty_tasks_.wait(lock);
        }
    }

protected:
    bool IsShutdown() const { return shutdown_.load(); }

    void StartShutdown() { shutdown_.store(true); }

    size_t GetTasks() const { return count_tasks.load(); }

    void AddTask() { count_tasks.fetch_add(1); }

    void RemoveTask() { count_tasks.fetch_sub(1); }

    bool CanCloseWorker() const {
        return shutdown_.load() && 0 == count_tasks.load();
    }

    void NotifyAll() { empty_tasks_.notify_all(); }
};

struct IExecutor : public Shutdowner {
    friend class NInternal::Worker;

    ~IExecutor() noexcept override = default;
    virtual void Submit(NExecutors::TaskBase* /*task*/) = 0;

protected:
    NComponents::SimpleMSQueue<TaskBase*> global_tasks;
};

}  // namespace NExecutors
