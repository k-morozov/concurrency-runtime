//
// Created by konstantin on 07.06.24.
//

#include "gtest/gtest.h"

#include <chrono>

#include <executor/pool/thread_pool.h>
#include <fiber/sync/async_mutex.h>
#include <go/go.h>
#include <common/clock.h>

using namespace std::chrono_literals;

namespace {
auto Now() { return std::chrono::high_resolution_clock::now(); }
}  // namespace

TEST(TestMutex, JustWorks1Go) {
    executors::ThreadPool scheduler{4};
    scheduler.Start();

    fibers::AsyncMutex mutex;
    size_t cs = 0;

    constexpr size_t expected = 17;

    fibers::Go(scheduler, [&] {
        for (size_t j = 0; j < expected; ++j) {
            std::lock_guard guard(mutex);
            ++cs;
        }
    });

    scheduler.WaitIdle();

    ASSERT_EQ(cs, expected);
}

TEST(TestMutex, Counter) {
    executors::ThreadPool scheduler{4};
    scheduler.Start();

    fibers::AsyncMutex mutex;
    size_t cs = 0;

    static const size_t kFibers = 10;
    static const size_t kSectionsPerFiber = 1024;

    for (size_t i = 0; i < kFibers; ++i) {
        fibers::Go(scheduler, [&] {
            for (size_t j = 0; j < kSectionsPerFiber; ++j) {
                std::lock_guard guard(mutex);
                ++cs;
            }
        });
    }

    scheduler.WaitIdle();

    std::cout << "# cs = " << cs
              << " (expected = " << kFibers * kSectionsPerFiber << ")"
              << std::endl;

    ASSERT_EQ(cs, kFibers * kSectionsPerFiber);
}

TEST(TestMutex, DoNotWasteCpu) {
    executors::ThreadPool scheduler{4};
    scheduler.Start();

    fibers::AsyncMutex mutex;

    const auto start = Now();

    common::ProcessCPUTimer timer;

    fibers::Go(scheduler, [&] {
        mutex.Lock();
        std::this_thread::sleep_for(1s);
        mutex.Unlock();
    });

    fibers::Go(scheduler, [&] {
        mutex.Lock();
        mutex.Unlock();
    });

    scheduler.WaitIdle();

    ASSERT_LE(timer.Elapsed(), 100ms);
}