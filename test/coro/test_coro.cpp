//
// Created by konstantin on 10.05.24.
//

#include "gtest/gtest.h"

#include <memory>

#include <coro/coro.h>

using namespace go::impl::coro;

TEST(TestCoro, JustWorks1Coro) {
    Coroutine co([] { Coroutine::Suspend(); });

    ASSERT_FALSE(co.IsCompleted());
    co.Resume();
    ASSERT_FALSE(co.IsCompleted());
    co.Resume();
    ASSERT_TRUE(co.IsCompleted());
}

TEST(TestCoro, Interleaving) {
    int step = 0;

    Coroutine a([&] {
        ASSERT_EQ(step, 0);
        step = 1;
        Coroutine::Suspend();
        ASSERT_EQ(step, 2);
        step = 3;
    });

    Coroutine b([&] {
        ASSERT_EQ(step, 1);
        step = 2;
        Coroutine::Suspend();
        ASSERT_EQ(step, 3);
        step = 4;
    });

    a.Resume();
    b.Resume();

    ASSERT_EQ(step, 2);

    a.Resume();
    b.Resume();

    ASSERT_TRUE(a.IsCompleted());
    ASSERT_TRUE(b.IsCompleted());

    ASSERT_EQ(step, 4);
}

TEST(TestCoro, MemoryLeak) {
    auto shared_ptr = std::make_shared<int>(42);
    std::weak_ptr<int> weak_ptr = shared_ptr;

    {
        auto routine = [ptr = std::move(shared_ptr)] {};
        Coroutine co(routine);
        co.Resume();
    }

    ASSERT_FALSE(weak_ptr.lock());
}