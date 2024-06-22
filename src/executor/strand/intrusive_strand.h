//
// Created by konstantin on 17.06.24.
//

#pragma once

#include <list>
#include <mutex>

#include <executor/executor.h>
#include <components/sync/spinLock.h>
#include <components/intrusive/list.h>

namespace executors {

class IntrusiveStrand : public IExecutor {
public:
    using TaskList = std::list<TaskPtr>;

    explicit IntrusiveStrand(IExecutor& underlying);

    IntrusiveStrand(const IntrusiveStrand&) = delete;
    IntrusiveStrand& operator=(const IntrusiveStrand&) = delete;

    IntrusiveStrand(IntrusiveStrand&&) = delete;
    IntrusiveStrand& operator=(IntrusiveStrand&&) = delete;

    void Submit(TaskPtr task) override;
    void Submit(NExecutors::TaskBase* task) override;

private:
    IExecutor& underlying;
    NSync::SpinLock spinlock;
    intrusive::List<NExecutors::TaskBase> tasks;

    std::atomic<bool> is_schedule{false};

    void SubmitInternal();
};

}  // namespace executors