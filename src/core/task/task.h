//
// Created by konstantin on 10.05.24.
//

#pragma once

#include <exception>

namespace NExe {

class Task {
public:
    virtual ~Task() = default;

    virtual void Run() = 0;
    virtual void Wait() {};
    virtual bool Cancel() const { return true; }

    virtual bool IsCompleted() const { return true; }
    virtual bool IsCanceled() const { return true; }
    virtual bool IsFinished() const { return true; }
    virtual bool IsFailed() const { return true; }

    std::exception_ptr GetError() { return nullptr; }
};

}  // namespace NExe