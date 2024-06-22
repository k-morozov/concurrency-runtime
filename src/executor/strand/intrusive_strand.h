//
// Created by konstantin on 17.06.24.
//

#pragma once

#include <list>
#include <mutex>

#include <components/intrusive/list.h>
#include <components/sync/spinLock.h>
#include <executor/executor.h>

namespace NExecutors {

class IntrusiveStrand : public IExecutor {
    IExecutor& underlying;
    NSync::SpinLock spinlock;
    intrusive::List<TaskBase> tasks;

    std::atomic<bool> is_schedule{false};

public:
    using TaskList = std::list<TaskPtr>;

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