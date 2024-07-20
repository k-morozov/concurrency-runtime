//
// Created by konstantin on 19.07.24.
//

#include "async_mutex.h"

#include <coroutine>
#include <iostream>

#include <components/async_mutex/mutex_awaiter.h>

namespace NComponents {

AsyncMutex::AsyncMutex(): event() {}

void AsyncMutex::lock() {
    std::cout << "[AsyncMutex::lock] call" << std::endl;
    LockImpl();
}

void AsyncMutex::unlock() {
    std::cout << "[AsyncMutex::unlock] call" << std::endl;
    event.UnSet();
}

ResumableNoOwn AsyncMutex::LockImpl() {
    std::cout << "[AsyncMutex::LockImpl] call" << std::endl;
    co_await event;
}

}  // namespace NComponents