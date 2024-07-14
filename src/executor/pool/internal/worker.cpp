//
// Created by konstantin on 24.06.24.
//

#include "worker.h"

#include <executor/executor.h>
#include <executor/task/task_base.h>

namespace NExecutors::NInternal {

using namespace std::chrono_literals;

namespace {
thread_local IExecutor* CurrentPool;

constexpr size_t MaxEmptyTasksInLoop = 3;
constexpr auto EmptyTasksSleepTimeout = 400ms;
}

Worker::Worker(IExecutor* ex) : ex(ex) {}

Worker::~Worker() {
    Join();
    assert(0 == ex->GetTasks());
}

void Worker::Start() {
    thread.emplace([this] {Loop(); });
}

void Worker::Join() {
    if (thread && thread->joinable()) {
        thread->join();
    }
}

void Worker::Push(TaskBase* task) {
    //    if (ex->IsShutdown()) return;

    local_tasks.Push(task);
    counter_empty_tasks.store(0);
    smph.release(1);
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
            counter_empty_tasks.fetch_sub(1);
            if (counter_empty_tasks.load() >= MaxEmptyTasksInLoop)
                coro->Suspend();
        }
        ex->NotifyAll();
    }
}
void Worker::Loop() {
    while (true) {
        if (!coro) {
            coro.emplace([this]() { Process(); },
                         NFibers::NContext::Buffer::AllocBytes(64 * 1024));
        }
        coro->Resume();
        if (ex->CanCloseWorker()) break;

        smph.try_acquire_for(EmptyTasksSleepTimeout);
    }
}

}  // namespace NExecutors::NInternal