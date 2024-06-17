//
// Created by konstantin on 17.06.24.
//

#pragma once

#include <deque>

#include <executor/executor.h>
#include <sync/spinLock.h>

namespace executors {

class Strand : public IExecutor {
public:
    explicit Strand(IExecutor& underlying);

    Strand(const Strand&) = delete;
    Strand& operator=(const Strand&) = delete;

    Strand(Strand&&) = delete;
    Strand& operator=(Strand&&) = delete;

    void Submit(TaskPtr task) override;

private:
    IExecutor& underlying;
    NSync::SpinLock spinlock;
    std::deque<TaskPtr> tasks;
};

}  // namespace executors