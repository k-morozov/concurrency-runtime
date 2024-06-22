//
// Created by konstantin on 22.06.24.
//

#pragma once

#include <queue>

#include <executor/executor.h>
#include <components/sync/spinLock.h>

namespace executors {

class IntrusiveManualExecutor final : public IExecutor {
public:
    IntrusiveManualExecutor() = default;

    [[deprecated]]
    void Submit(TaskPtr /*task*/) override;

    void Submit(NExecutors::TaskBase* /*task*/) override;

    size_t RunAtMost(size_t limit);

    bool RunNext() { return 1 == RunAtMost(1); }

    size_t Drain();

    size_t TaskCount() const;

    bool IsEmpty() const;

    bool NonEmpty() const;

private:
    mutable NSync::SpinLock spinlock;
    intrusive::List<NExecutors::TaskBase> tasks;
    size_t count_tasks{};
};

}  // namespace executors
