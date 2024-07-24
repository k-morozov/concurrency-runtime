//
// Created by konstantin on 19.07.24.
//

#pragma once

#include <coroutine>
#include <iostream>
#include <mutex>

#include <components/sync/spinLock.h>

namespace NComponents {

struct Event;
using LockGuard = std::unique_lock<NSync::SpinLock>;

class MutexAwaiter final {
    Event& event;
    mutable LockGuard guard;
    std::coroutine_handle<> coro{};

public:
    MutexAwaiter(Event& event, LockGuard guard);
    MutexAwaiter(MutexAwaiter&& o) noexcept;

    ~MutexAwaiter();

    void Resume() const { coro.resume(); }
    void ReleaseLock() const;
    bool HasLock() const;

    bool await_ready() const;
    void await_suspend(std::coroutine_handle<>) noexcept;
    void await_resume() const noexcept;
};

std::ostream& operator<<(std::ostream& stream, const MutexAwaiter& w);

}  // namespace NComponents