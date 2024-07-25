//
// Created by konstantin on 19.07.24.
//

#pragma once

#include <coroutine>
#include <iostream>
#include <mutex>

#include <components/sync/spinLock.h>

namespace NComponents {

class AsyncMutexCoroImpl;

class MutexAwaiter final {
public:
    using LockGuard = std::unique_lock<NSync::SpinLock>;

    MutexAwaiter(AsyncMutexCoroImpl& event, LockGuard&& guard);
    MutexAwaiter(MutexAwaiter&& o) noexcept;

    ~MutexAwaiter();

    void Resume() const { coro.resume(); }
    void ReleaseLock() const;
//    bool HasLock() const;

    bool await_ready() const;
    void await_suspend(std::coroutine_handle<>) noexcept;
    void await_resume() const noexcept;

private:
    AsyncMutexCoroImpl& event;
    mutable LockGuard guard;
    std::coroutine_handle<> coro{};
};

std::ostream& operator<<(std::ostream& stream, const MutexAwaiter& w);

}  // namespace NComponents