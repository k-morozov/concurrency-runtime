//
// Created by konstantin on 29.05.24.
//

#pragma once

#include <functional>

#include <executor/task/task.h>

namespace executors {

class FiberTask : public Task {
public:
    explicit FiberTask(std::function<void()>&& f) : f_(std::move(f)){};
    void Run() override { f_(); }

private:
    std::function<void()> f_;
};

}  // namespace executors
