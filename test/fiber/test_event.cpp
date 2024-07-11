//
// Created by konstantin on 10.06.24.
//

#include "gtest/gtest.h"

#include <common/clock.h>
#include <executor/pool/intrusive_pool.h>
#include <fiber/sync/event.h>
#include <go/go.h>

using namespace std::chrono_literals;

TEST(TestEvent, OneWaiter) {
    NExecutors::IntrusiveThreadPool scheduler{4};
    scheduler.Start();

    static const std::string kMessage = "Hello";

    NFibers::Event event;
    std::string data;
    bool ok = false;

    NFibers::Go(scheduler, [&] {
        event.Wait();
        ASSERT_EQ(data, kMessage);
        ok = true;
    });

    std::this_thread::sleep_for(1s);

    NFibers::Go(scheduler, [&] {
        data = kMessage;
        event.Fire();
    });

    scheduler.WaitIdle();

    ASSERT_TRUE(ok);
}

TEST(TestEvent, DoNotBlockThread) {
    NExecutors::IntrusiveThreadPool scheduler{1};
    scheduler.Start();

    NFibers::Event event;
    bool ok = false;

    NFibers::Go(scheduler, [&] {
        event.Wait();
        ok = true;
    });

    NFibers::Go(scheduler, [&] {
        for (size_t i = 0; i < 10; ++i) {
            std::this_thread::sleep_for(32ms);
            NFibers::Yield();
        }
        event.Fire();
    });

    scheduler.WaitIdle();

    ASSERT_TRUE(ok);
}

TEST(TestEvent, MultipleWaiters) {
    NExecutors::IntrusiveThreadPool scheduler{1};
    scheduler.Start();

    NFibers::Event event;
    std::atomic<size_t> waiters{0};

    static const size_t kWaiters = 7;

    for (size_t i = 0; i < kWaiters; ++i) {
        NFibers::Go(scheduler, [&] {
            event.Wait();
            ++waiters;
        });
    }

    std::this_thread::sleep_for(1s);

    NFibers::Go(scheduler, [&] { event.Fire(); });

    scheduler.WaitIdle();

    ASSERT_EQ(waiters.load(), kWaiters);
}

TEST(TestEvent, DISABLED_DoNotWasteCpu) {
    NExecutors::IntrusiveThreadPool scheduler{4};
    scheduler.Start();

    common::ProcessCPUTimer cpu_timer;

    NFibers::Event event;

    NFibers::Go(scheduler, [&] { event.Wait(); });

    std::this_thread::sleep_for(1s);

    NFibers::Go(scheduler, [&] { event.Fire(); });

    scheduler.WaitIdle();

    ASSERT_TRUE(cpu_timer.Spent() < 100ms);
}