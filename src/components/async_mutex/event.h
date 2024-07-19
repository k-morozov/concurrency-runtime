//
// Created by konstantin on 19.07.24.
//

#pragma once

#include <atomic>

#include <components/async_mutex/mutex_awaiter.h>

namespace NComponents {
struct Event final {
    std::atomic<bool>& flag;

    explicit Event(std::atomic<bool>& flag) : flag(flag) {}

    MutexAwaiter operator co_await();

    void ParkAwaiter();
};
}  // namespace NComponents