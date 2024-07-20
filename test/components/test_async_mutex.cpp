//
// Created by konstantin on 20.07.24.
//

#include "gtest/gtest.h"

#include <thread>

#include <components/async_mutex/async_mutex.h>

using namespace std::chrono_literals;

//TEST(TestAsyncMutex, JustWorking) {
//    NComponents::AsyncMutex mutex;
//
//    mutex.lock();
//    mutex.unlock();
//}

TEST(TestAsyncMutex, SomeThreads) {
    NComponents::AsyncMutex mutex;
    size_t number{};

    constexpr size_t MaxCount = 32;
    {
        std::vector<std::optional<std::jthread>> workers;
        for(size_t i=0; i<MaxCount; i++) {
            workers.emplace_back([&]{
                mutex.lock();
                std::this_thread::sleep_for(10ms);
                number += 1;
                mutex.unlock();
            });
        }
    }

    ASSERT_EQ(number, MaxCount);
}