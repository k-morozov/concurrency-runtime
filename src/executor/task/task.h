//
// Created by konstantin on 10.05.24.
//

#pragma once

#include <condition_variable>
#include <exception>
#include <mutex>

namespace executors {

class Task;
using TaskPtr = std::shared_ptr<Task>;

class Task {
public:
    virtual ~Task() = default;

    virtual void Run() = 0;

    void Cancel();

    void Wait();
    void MarkCompleted();
    void MarFailed(std::exception_ptr);

    bool IsCompleted() const;
    bool IsCanceled() const;
    bool IsFinished() const;
    bool IsFailed() const;

    std::exception_ptr GetError();

private:
    enum class Status {
        NotStarted,
        Completed,
        Cancelled,
        Failed,
    };

    mutable std::mutex task_mutex_;
    Status status_{Status::NotStarted};
    std::condition_variable change_task_status_;

    std::exception_ptr ex_error_{};
};

}  // namespace executors