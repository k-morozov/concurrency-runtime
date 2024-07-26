//
// Created by konstantin on 19.07.24.
//

#pragma once

#include <atomic>
#include <coroutine>

#include <components/async_mutex/async_mutex_coro_impl.h>
#include <components/async_mutex/resumable_no_own.h>

namespace NComponents {

class AsyncMutex final {
public:
    AsyncMutex();
    ~AsyncMutex();

    AsyncMutexCoroImpl& lock();
    void unlock();

private:
    AsyncMutexCoroImpl mutex_impl;
};

}  // namespace NComponents
