//
// Created by konstantin on 19.07.24.
//

#pragma once

#include <atomic>
#include <coroutine>

#include <components/async_mutex/event.h>
#include <components/async_mutex/resumable_no_own.h>

namespace NComponents {

class AsyncMutex final {
public:
    AsyncMutex();

    void lock();
    void unlock();

private:
    Event event;

    ResumableNoOwn LockImpl();
};

}  // namespace NComponents
