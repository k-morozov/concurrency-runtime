//
// Created by konstantin on 10.06.24.
//

#include "gtest/gtest.h"

#include <common/clock.h>
#include <executor/pool/thread_pool.h>
#include <fiber/sync/event.h>
#include <go/go.h>

using namespace std::chrono_literals;

TEST(TestEvent, OneWaiter) {
    executors::ThreadPool scheduler{4};
    scheduler.Start();

    static const std::string kMessage = "Hello";

    fibers::Event event;
    std::string data;
    bool ok = false;

    fibers::Go(scheduler, [&] {
        event.Wait();
        ASSERT_EQ(data, kMessage);
        ok = true;
    });

    std::this_thread::sleep_for(1s);

    fibers::Go(scheduler, [&] {
        data = kMessage;
        event.Fire();
    });

    scheduler.WaitIdle();

    ASSERT_TRUE(ok);
}

TEST(TestEvent, DoNotBlockThread) {
    executors::ThreadPool scheduler{1};
    scheduler.Start();

    fibers::Event event;
    bool ok = false;

    fibers::Go(scheduler, [&] {
        event.Wait();
        ok = true;
    });

    fibers::Go(scheduler, [&] {
        for (size_t i = 0; i < 10; ++i) {
            std::this_thread::sleep_for(32ms);
            fibers::Yield();
        }
        event.Fire();
    });

    scheduler.WaitIdle();

    ASSERT_TRUE(ok);
}

TEST(TestEvent, MultipleWaiters) {
    executors::ThreadPool scheduler{1};
    scheduler.Start();

    fibers::Event event;
    std::atomic<size_t> waiters{0};

    static const size_t kWaiters = 7;

    for (size_t i = 0; i < kWaiters; ++i) {
        fibers::Go(scheduler, [&] {
            event.Wait();
            ++waiters;
        });
    }

    std::this_thread::sleep_for(1s);

    fibers::Go(scheduler, [&] { event.Fire(); });

    scheduler.WaitIdle();

    ASSERT_EQ(waiters.load(), kWaiters);
}

TEST(TestEvent, DoNotWasteCpu) {
    executors::ThreadPool scheduler{4};
    scheduler.Start();

    common::ProcessCPUTimer cpu_timer;

    fibers::Event event;

    fibers::Go(scheduler, [&] { event.Wait(); });

    std::this_thread::sleep_for(1s);

    fibers::Go(scheduler, [&] { event.Fire(); });

    scheduler.WaitIdle();

    ASSERT_TRUE(cpu_timer.Spent() < 100ms);
}