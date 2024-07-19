//
// Created by konstantin on 19.07.24.
//

#pragma once

#include <atomic>
#include <coroutine>

#include <components/async_mutex/event.h>

namespace NComponents {

class AsyncMutex final {
public:
    AsyncMutex();

    void lock();
    void unlock();

private:
    std::atomic<bool> flag{};
    Event event;
};

}  // namespace NComponents
