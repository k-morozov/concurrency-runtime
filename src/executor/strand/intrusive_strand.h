//
// Created by konstantin on 17.06.24.
//

#pragma once

#include <mutex>

#include <components/intrusive/list.h>
#include <components/sync/spinLock.h>
#include <executor/executor.h>

namespace NExecutors {

class IntrusiveStrand : public IExecutor {
    using TaskList = NComponents::IntrusiveList<TaskBase>;

    enum class State : uint32_t {
        READY,
        PLAN,
        RUNNING
    };

    IExecutor& underlying;
    mutable NSync::SpinLock spinlock;
    TaskList tasks;

    std::atomic<State> state{State::READY};

public:
    explicit IntrusiveStrand(IExecutor& underlying);

    IntrusiveStrand(const IntrusiveStrand&) = delete;
    IntrusiveStrand& operator=(const IntrusiveStrand&) = delete;

    IntrusiveStrand(IntrusiveStrand&&) = delete;
    IntrusiveStrand& operator=(IntrusiveStrand&&) = delete;

    void Submit(TaskBase* task, bool is_internal) override;

private:
    void SubmitInternal();

    void RunBatch();

    void PlanTask(State from);

    bool ChangeState(State from, State to);

    bool IsTasksEmpty() const {
        std::lock_guard lock(spinlock);
        return tasks.IsEmpty();
    }
};

}  // namespace NExecutors