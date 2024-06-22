//
// Created by konstantin on 19.06.24.
//

#pragma once

#include <components/intrusive/list.h>

namespace NExecutors {

struct ITask {
    virtual ~ITask() = default;
    virtual void Run() noexcept = 0;
};

class TaskBase : public ITask, public intrusive::Node<TaskBase> {
public:
private:
};

}  // namespace NExecutors
