//
// Created by konstantin on 04.06.24.
//

#pragma once

#include <queue>

#include <executor/executor.h>

namespace executors {

class ManualExecutor final : public IExecutor {
public:
    ManualExecutor() = default;
    void Submit(TaskPtr /*task*/) override;

    size_t RunAtMost(size_t limit);

    bool RunNext() { return 1 == RunAtMost(1); }

    size_t Drain();

    size_t TaskCount() const;

    bool IsEmpty() const;

    bool NonEmpty() const;

private:
    std::queue<TaskPtr> tasks_;
};

}  // namespace executors
