//
// Created by konstantin on 24.06.24.
//

#include "worker.h"

#include <executor/executor.h>
#include <executor/task/task_base.h>

namespace NExecutors::NInternal {

namespace {
thread_local IExecutor* CurrentPool;
}

Worker::Worker(IExecutor* ex)
    : ex(ex),
      coro([this]() { Process(); },
           NFibers::NContext::Buffer::AllocBytes(64 * 1024)) {}

Worker::~Worker() {
    Join();
    assert(0 == ex->GetTasks());
}

void Worker::Start() {
    thread.emplace(std::thread([this]() { coro.Resume(); }));
}

void Worker::Join() {
    if (thread && thread->joinable()) {
        thread->join();
    }
}

void Worker::Push(TaskBase* task) {
    //    if (ex->IsShutdown()) return;

    local_tasks.Push(task);
}

IExecutor* Worker::Current() { return CurrentPool; }

void Worker::Process() {
    auto worker_mutator =
        NComponents::NHazard::HazardManager::Get()->MakeMutator();

    while (true) {
        CurrentPool = ex;
        TaskBase* task{};
        {
            auto res = local_tasks.TryPop(worker_mutator);
            if (res) {
                task = res.value();
            }
        }

        if (task) {
            const auto status = task->Run();
            if (status == ITask::TaskRunResult::COMPLETE) {
                ex->RemoveTask();
            }

        } else {
            if (ex->CanCloseWorker()) {
                break;
            }
        }
        ex->NotifyAll();
    }
}

}  // namespace NExecutors::NInternal