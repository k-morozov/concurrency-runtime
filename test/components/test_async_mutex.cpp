//
// Created by konstantin on 20.07.24.
//

#include "gtest/gtest.h"

#include <thread>

#include <components/async_mutex/async_mutex.h>


TEST(TestAsyncMutex, JustWorking) {
    NComponents::AsyncMutex mutex;

    mutex.lock();
    mutex.unlock();
}

TEST(TestAsyncMutex, OneThread) {
    NComponents::AsyncMutex mutex;
    int number{};

    {
        std::jthread th1([&]{
            mutex.lock();
            number += 1000;
            mutex.unlock();
        });
        std::jthread th2([&]{
            mutex.lock();
            number += 100;
            mutex.unlock();
        });
        std::jthread th3([&]{
            mutex.lock();
            number += 10;
            mutex.unlock();
        });
        std::jthread th4([&]{
            mutex.lock();
            number += 1;
            mutex.unlock();
        });
    }

    ASSERT_EQ(number, 1111);
}