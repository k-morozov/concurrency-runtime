//
// Created by konstantin on 19.07.24.
//

#include "async_mutex.h"

#include <coroutine>
#include <syncstream>
#include <thread>

#include <components/async_mutex/mutex_awaiter.h>

namespace NComponents {

AsyncMutex::AsyncMutex() : mutex_impl() {}

AsyncMutexCoroImpl& AsyncMutex::lock() {
    return mutex_impl;
}

AsyncMutex::~AsyncMutex() {
    assert(event.waiters.empty());
}

void AsyncMutex::unlock() {
    mutex_impl.Unlock();
}

}  // namespace NComponents