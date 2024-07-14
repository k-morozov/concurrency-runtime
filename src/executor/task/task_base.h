//
// Created by konstantin on 19.06.24.
//

#pragma once

#include <stdexcept>

#include <components/intrusive/list.h>

namespace NExecutors {

enum class StateTask : unsigned { PLANNED, RUNNING, FINISHED };

enum class SchedulerHint : bool { GLOBAL, LOCAL };

namespace NInternal {
class Worker;
}

struct ITask {
    enum class TaskRunResult : bool { UNCOMPLETED, COMPLETE };

    virtual ~ITask() = default;
    virtual TaskRunResult Run() noexcept = 0;
};

class TaskBase : public ITask, public NComponents::Node<TaskBase> {
public:
    [[nodiscard]] StateTask GetState() const { return state; }
    void ProgressState() {
        switch (state) {
            case StateTask::PLANNED:
                state = StateTask::RUNNING;
                break;
            case StateTask::RUNNING:
                state = StateTask::FINISHED;
                break;
            case StateTask::FINISHED:
                throw std::runtime_error("Task can't progress after finished");
        }
    }

    void SetWorker(NInternal::Worker* w) {
        if (!worker) worker = w;
    }

    NInternal::Worker* GetWorker() { return worker; }

private:
    StateTask state{StateTask::PLANNED};
    NInternal::Worker* worker{};
};

}  // namespace NExecutors
