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

constexpr size_t MaxEmptyTasksInLoop = 2;
constexpr auto EmptyTasksSleepTimeout = 400ms;
}  // namespace

Worker::Worker(IExecutor* ex) : ex(ex) {}

Worker::~Worker() {
    shutdown.store(true);
    smph.release(1);

    Join();
    assert(0 == ex->GetTasks());
}

void Worker::Start() {
    thread.emplace([this] { Loop(); });
}

void Worker::Join() {
    if (thread && thread->joinable()) {
        thread->join();
    }
}

void Worker::Push(TaskBase* task) {
    local_tasks.Push(task);

    counter_empty_tasks.store(0);
    ex->WakeUpSuspendedWorker();
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
            auto local_task = local_tasks.TryPop(worker_mutator);
            if (local_task) {
                task = local_task.value();
            }
        }

        if (task) {
            task->SetWorker(this);
            const auto status = task->Run();
            if (status == ITask::TaskRunResult::COMPLETE) {
                ex->RemoveTask();
            }
        } else {
            {
                auto global_task = ex->global_tasks.TryPop(worker_mutator);
                if (global_task) {
                    Push(*global_task);
                } else {
                    if (ex->CanCloseWorker()) {
                        break;
                    }

                    counter_empty_tasks.fetch_sub(1);
                    if (counter_empty_tasks.load() >= MaxEmptyTasksInLoop &&
                        !shutdown.load()) {
                        ex->AddSuspendedWorker();
                        coro->Suspend();
                    }
                }
            }
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

        if (!shutdown.load()) smph.try_acquire_for(EmptyTasksSleepTimeout);
    }
}

}  // namespace NExecutors::NInternal