//
// Created by konstantin on 09.05.24.
//

#pragma once

#include <core/task/task.h>

#include <memory>

namespace NExe {

class Executor;
std::shared_ptr<Executor> MakeThreadPool();

class Executor {
public:
    explicit Executor() = default;
    ~Executor() = default;

    void Submit(std::shared_ptr<Task> /*task*/) {}

    void StartShutdown() {}

    void WaitShutdown() {}
};

}  // namespace NExe