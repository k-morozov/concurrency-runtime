//
// Created by konstantin on 24.06.24.
//

#include "worker.h"

#include <executor/executor.h>
#include <executor/task/task_base.h>

namespace NExecutors::internal {

namespace {
thread_local IExecutor* CurrentPool;
}

Worker::Worker(IExecutor* ex) : ex(ex) {}

Worker::~Worker() {
    Join();
    assert(0 == ex->count_tasks.load());
}

void Worker::Start() {
    thread.emplace(std::thread([this]() {
        while (true) {
            CurrentPool = ex;
            TaskBase* task{};
            {
                auto res = local_tasks.TryPop();
                if (res) {
                    task = res.value();
                }
            }

            if (task) {
                const auto status = task->Run();
                if (status == ITask::TaskRunResult::COMPLETE) {
                    ex->count_tasks.fetch_sub(1);
                }

            } else {
                if (ex->shutdown_.load() && 0 == ex->count_tasks.load()) {
                    break;
                }
            }
            ex->empty_tasks_.notify_all();
        }
    }));
}

void Worker::Join() {
    if (thread && thread->joinable()) {
        thread->join();
    }
}

void Worker::Push(TaskBase* task, const bool is_internal) {
    if (!is_internal) {
        if (ex->shutdown_.load()) return;
    }

    local_tasks.Push(task);
}

IExecutor* Worker::Current() { return CurrentPool; }

}  // namespace NExecutors::internal