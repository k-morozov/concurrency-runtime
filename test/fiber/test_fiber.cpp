//
// Created by konstantin on 10.05.24.
//

#include "gtest/gtest.h"

#include <chrono>

#include <go/go.h>

using namespace std::chrono_literals;

TEST(TestFiber, JustWorks1Go) {
    pool::Executor pool{3};

    bool done = false;

    Go(pool, [&]() { done = true; });

    pool.StartShutdown();
    pool.WaitShutdown();

    ASSERT_TRUE(done);
}

TEST(TestFiber, Child) {
    pool::Executor pool{3};

    std::atomic<size_t> done{0};

    auto init = [&]() {
        Go([&]() { ++done; });

        ++done;
    };

    Go(pool, init);

    // @todo change API of thread pool
    std::this_thread::sleep_for(1s);
    //    pool.WaitShutdown();

    ASSERT_EQ(done.load(), 2u);

    pool.StartShutdown();
    pool.WaitShutdown();
}