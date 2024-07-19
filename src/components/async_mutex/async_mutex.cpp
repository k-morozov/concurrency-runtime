//
// Created by konstantin on 19.07.24.
//

#include "async_mutex.h"

#include <coroutine>

#include <components/async_mutex/mutex_awaiter.h>

namespace NComponents {

AsyncMutex::AsyncMutex(): event(flag) {}

void AsyncMutex::lock() {}

void AsyncMutex::unlock() {}

}  // namespace NComponents