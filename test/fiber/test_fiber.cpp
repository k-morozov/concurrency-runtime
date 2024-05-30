//
// Created by konstantin on 10.05.24.
//

#include "gtest/gtest.h"

#include <go/go.h>


TEST(TestFiber, JustWorks1Go) {
    pool::Executor pool{3};

    bool done = false;

    Go(pool, [&]() {
        done = true;
    });

    pool.StartShutdown();
    pool.WaitShutdown();

    ASSERT_TRUE(done);
}

TEST(TestFiber, Child) {
    pool::Executor pool{3};

    std::atomic<size_t> done{0};

    auto init = [&]() {
        Go([&]() {
            ++done;
        });

        ++done;
    };

    Go(pool, init);

    pool.StartShutdown();
    pool.WaitShutdown();

    ASSERT_EQ(done.load(), 2u);
}