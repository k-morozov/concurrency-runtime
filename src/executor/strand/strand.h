//
// Created by konstantin on 17.06.24.
//

#pragma once

#include <list>
#include <mutex>

#include <executor/executor.h>
#include <sync/spinLock.h>

namespace executors {

class Strand : public IExecutor {
public:
    using TaskList = std::list<TaskPtr>;

    explicit Strand(IExecutor& underlying);

    Strand(const Strand&) = delete;
    Strand& operator=(const Strand&) = delete;

    Strand(Strand&&) = delete;
    Strand& operator=(Strand&&) = delete;

    void Submit(TaskPtr task) override;

private:
    IExecutor& underlying;
    NSync::SpinLock spinlock;
    TaskList tasks;

    std::atomic<bool> is_schedule{false};

    void SubmitInternal();
};

}  // namespace executors