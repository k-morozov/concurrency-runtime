//
// Created by konstantin on 20.07.24.
//

#include "gtest/gtest.h"

#include <thread>

#include <components/async_mutex/async_mutex.h>

using namespace std::chrono_literals;

TEST(TestAsyncMutex, JustWorking) {
    NComponents::AsyncMutex mutex;

    mutex.lock();
    mutex.unlock();
}

TEST(TestAsyncMutex, SomeThreads) {
    NComponents::AsyncMutex mutex;
    int number{};

    {
        std::vector<std::optional<std::jthread>> workers;
        for(size_t i=0; i<2; i++) {
            workers.emplace_back([&]{
                mutex.lock();
                std::this_thread::sleep_for(10ms);
                number += 1;
                mutex.unlock();
            });
        }
    }

    ASSERT_EQ(number, 4);
}