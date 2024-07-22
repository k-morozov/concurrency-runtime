//
// Created by konstantin on 19.07.24.
//

#include "async_mutex.h"

#include <coroutine>
#include <syncstream>
#include <thread>

#include <components/async_mutex/mutex_awaiter.h>

namespace NComponents {

AsyncMutex::AsyncMutex(): event() {}

void AsyncMutex::lock() {
    std::osyncstream(std::cout) << "[AsyncMutex::lock][thread_id=" << std::this_thread::get_id() << "] call" << std::endl;
    LockImpl();
}

void AsyncMutex::unlock() {
    std::osyncstream(std::cout) << "[AsyncMutex::unlock][thread_id=" << std::this_thread::get_id() << "] call" << std::endl;
    event.UnSet();
}

ResumableNoOwn AsyncMutex::LockImpl() {
    std::osyncstream(std::cout) << "[AsyncMutex::LockImpl] Start in thread_id=" << std::this_thread::get_id() << std::endl;
    co_await event;
    std::osyncstream(std::cout) << "[AsyncMutex::LockImpl] Resumed in thread_id=" << std::this_thread::get_id() << std::endl;
}

}  // namespace NComponents