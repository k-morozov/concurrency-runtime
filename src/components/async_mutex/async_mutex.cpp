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

void AsyncMutex::unlock() {
    std::osyncstream(std::cout)
        << "[AsyncMutex::unlock][thread_id=" << std::this_thread::get_id()
        << "] call" << std::endl;
    event.UnSet();
}

}  // namespace NComponents