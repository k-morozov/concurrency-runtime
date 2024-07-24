//
// Created by konstantin on 19.07.24.
//

#include "async_mutex.h"

#include <coroutine>
#include <syncstream>
#include <thread>

#include <components/async_mutex/mutex_awaiter.h>

namespace NComponents {

AsyncMutex::AsyncMutex() : event() {}

Event& AsyncMutex::lock() {
    return event;
}

AsyncMutex::~AsyncMutex() {
    assert(event.waiters.empty());
}

void AsyncMutex::unlock() {
    event.Unlock();
}

}  // namespace NComponents