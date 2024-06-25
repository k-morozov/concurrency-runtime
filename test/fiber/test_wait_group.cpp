//
// Created by konstantin on 09.06.24.
//
#include "gtest/gtest.h"

#include <common/clock.h>
#include <executor/pool/intrusive_pool.h>
#include <executor/submit.h>
#include <fiber/sync/wait_group.h>
#include <go/go.h>

using namespace std::chrono_literals;

TEST(TestWaitGroup, JustWorks) {
    NExecutors::IntrusiveThreadPool pool{4};

    pool.Start();

    fibers::Go(pool, [] {
        fibers::WaitGroup wg;
        wg.Add(1);

        fibers::Go([&wg] {
            std::cout << "Hello from thread pool!" << std::endl;
            wg.Done();
        });

        wg.Wait();
    });
}

TEST(TestWaitGroup, OneWaiter) {
    NExecutors::IntrusiveThreadPool scheduler{5};
    scheduler.Start();

    fibers::WaitGroup wg;
    std::atomic<size_t> work{0};
    bool ok = false;

    static const size_t kWorkers = 3;

    wg.Add(kWorkers);

    fibers::Go(scheduler, [&] {
        wg.Wait();
        ASSERT_EQ(work.load(), kWorkers);
        ok = true;
    });

    for (size_t i = 0; i < kWorkers; ++i) {
        fibers::Go(scheduler, [&] {
            std::this_thread::sleep_for(1s);
            ++work;
            wg.Done();
        });
    }

    scheduler.WaitIdle();

    ASSERT_TRUE(ok);
}

TEST(TestWaitGroup, MultipleWaiters) {
    NExecutors::IntrusiveThreadPool scheduler{5};
    scheduler.Start();

    fibers::WaitGroup wg;

    std::atomic<size_t> work{0};
    std::atomic<size_t> acks{0};

    static const size_t kWorkers = 3;
    static const size_t kWaiters = 4;

    wg.Add(kWorkers);

    for (size_t i = 0; i < kWaiters; ++i) {
        fibers::Go(scheduler, [&] {
            wg.Wait();
            ASSERT_EQ(work.load(), kWorkers);
            ++acks;
        });
    }

    for (size_t i = 0; i < kWorkers; ++i) {
        fibers::Go(scheduler, [&] {
            std::this_thread::sleep_for(1s);
            ++work;
            wg.Done();
        });
    }

    scheduler.WaitIdle();

    ASSERT_EQ(acks.load(), kWaiters);
}

TEST(TestWaitGroup, DoNotBlockThread) {
    NExecutors::IntrusiveThreadPool scheduler{1};
    scheduler.Start();

    fibers::WaitGroup wg;
    bool ok = false;

    fibers::Go(scheduler, [&] {
        wg.Wait();
        ok = true;
    });

    wg.Add(1);

    fibers::Go(scheduler, [&] {
        for (size_t i = 0; i < 10; ++i) {
            std::this_thread::sleep_for(32ms);
            fibers::Yield();
        }
        wg.Done();
    });

    scheduler.WaitIdle();

    ASSERT_TRUE(ok);
}

TEST(TestWaitGroup, DISABLED_DoNotWasteCpu) {
    NExecutors::IntrusiveThreadPool scheduler{4};
    scheduler.Start();

    fibers::WaitGroup wg;

    std::atomic<size_t> workers = 0;

    common::ProcessCPUTimer cpu_timer;

    static const size_t kWorkers = 3;

    wg.Add(kWorkers);

    fibers::Go(scheduler, [&] {
        wg.Wait();
        ASSERT_EQ(workers.load(), kWorkers);
    });

    std::this_thread::sleep_for(1s);

    for (size_t i = 0; i < kWorkers; ++i) {
        fibers::Go(scheduler, [&] {
            ++workers;
            wg.Done();
        });
    }

    scheduler.WaitIdle();

    ASSERT_TRUE(cpu_timer.Spent() < 100ms);
}