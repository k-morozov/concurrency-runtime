//
// Created by konstantin on 19.06.24.
//

#pragma once

#include <stdexcept>

#include <components/intrusive/list.h>

namespace NExecutors {

enum class StateTask : unsigned { PLANNED, RUNNING, FINISHED };

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

private:
    StateTask state{StateTask::PLANNED};
};

}  // namespace NExecutors
