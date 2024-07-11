//
// Created by konstantin on 07.06.24.
//

#include "gtest/gtest.h"

#include <executor/pool/intrusive_pool.h>
#include <fiber/sync/async_mutex.h>
#include <go/go.h>
#include <common/clock.h>

using namespace std::chrono_literals;

TEST(TestMutex, JustWorks1Go) {
    NExecutors::IntrusiveThreadPool scheduler{4};
    scheduler.Start();

    NFibers::AsyncMutex mutex;
    size_t counter = 0;

    constexpr size_t expected = 17;

    NFibers::Go(scheduler, [&] {
        for (size_t j = 0; j < expected; ++j) {
            std::lock_guard guard(mutex);
            ++counter;
        }
    });

    scheduler.WaitIdle();

    ASSERT_EQ(counter, expected);
}

TEST(TestMutex, Counter) {
    NExecutors::IntrusiveThreadPool scheduler{4};
    scheduler.Start();

    NFibers::AsyncMutex mutex;
    size_t counter = 0;

    static const size_t kFibers = 8;
    static const size_t kSectionsPerFiber = 1024;

    for (size_t i = 0; i < kFibers; ++i) {
        NFibers::Go(scheduler, [&] {
            for (size_t j = 0; j < kSectionsPerFiber; ++j) {
                std::lock_guard guard(mutex);
                ++counter;
            }
        });
    }

    scheduler.WaitIdle();

    ASSERT_EQ(counter, kFibers * kSectionsPerFiber);
}

TEST(TestMutex, DISABLED_DoNotWasteCpu) {
    NExecutors::IntrusiveThreadPool scheduler{4};
    scheduler.Start();

    NFibers::AsyncMutex mutex;

    common::ProcessCPUTimer timer;

    NFibers::Go(scheduler, [&] {
        mutex.Lock();
        std::this_thread::sleep_for(1s);
        mutex.Unlock();
    });

    NFibers::Go(scheduler, [&] {
        mutex.Lock();
        mutex.Unlock();
    });

    scheduler.WaitIdle();

    ASSERT_LE(timer.Elapsed(), 100ms);
}