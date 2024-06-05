//
// Created by konstantin on 11.05.24.
//

#include "task.h"

#include <cassert>
#include <mutex>

namespace executors {

void Task::Cancel() {
    std::lock_guard lock(task_mutex_);
    assert(Status::NotStarted == status_);
    status_ = Status::Cancelled;

    change_task_status_.notify_one();
}

void Task::Wait() {
    std::unique_lock lock(task_mutex_);
    while (Status::NotStarted == status_) {
        change_task_status_.wait(lock);
    }
}

void Task::MarkCompleted() {
    std::lock_guard lock(task_mutex_);
    assert(Status::NotStarted == status_);
    status_ = Status::Completed;

    change_task_status_.notify_one();
}

void Task::MarFailed(std::exception_ptr p) {
    std::lock_guard lock(task_mutex_);
    assert(Status::NotStarted == status_);
    status_ = Status::Failed;
    ex_error_ = std::move(p);

    change_task_status_.notify_one();
}

bool Task::IsCompleted() const {
    std::lock_guard lock(task_mutex_);
    return Status::Completed == status_;
}

bool Task::IsCanceled() const {
    std::lock_guard lock(task_mutex_);
    return Status::Cancelled == status_;
}

bool Task::IsFinished() const {
    std::lock_guard lock(task_mutex_);
    return Status::NotStarted != status_;
}

bool Task::IsFailed() const {
    std::lock_guard lock(task_mutex_);
    return Status::Failed == status_;
}

std::exception_ptr Task::GetError() {
    std::lock_guard lock(task_mutex_);
    return ex_error_;
}

}  // namespace executors