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
    using TaskList = intrusive::List<TaskBase>;

    IExecutor& underlying;
    NSync::SpinLock spinlock;
    TaskList tasks;

    std::atomic<bool> is_schedule{false};

public:
    explicit IntrusiveStrand(IExecutor& underlying);

    IntrusiveStrand(const IntrusiveStrand&) = delete;
    IntrusiveStrand& operator=(const IntrusiveStrand&) = delete;

    IntrusiveStrand(IntrusiveStrand&&) = delete;
    IntrusiveStrand& operator=(IntrusiveStrand&&) = delete;

    void Submit(TaskBase* task) override;

private:
    void SubmitInternal();
};

}  // namespace NExecutors