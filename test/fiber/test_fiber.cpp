//
// Created by konstantin on 10.05.24.
//

#include "gtest/gtest.h"

#include <chrono>

#include <executor/pool/thread_pool.h>
#include <executor/pool/intrusive_pool.h>
#include <go/go.h>

using namespace std::chrono_literals;

namespace {
auto Now() { return std::chrono::high_resolution_clock::now(); }
}  // namespace

TEST(TestFiber, JustWorks1Go) {
    NExecutors::IntrusiveThreadPool pool{3};
    pool.Start();

    bool done = false;

    NFibers::Go(pool, [&]() {
        ASSERT_EQ(NExecutors::IntrusiveThreadPool::Current(), &pool);
        done = true;
    });

    pool.WaitIdle();

    ASSERT_TRUE(done);
}

TEST(TestFiber, Child) {
    NExecutors::IntrusiveThreadPool pool{3};
    pool.Start();

    std::atomic<size_t> done{0};

    auto init = [&]() {
        ASSERT_EQ(NExecutors::IntrusiveThreadPool::Current(), &pool);

        NFibers::Go([&]() {
            ASSERT_EQ(NExecutors::IntrusiveThreadPool::Current(), &pool);
            ++done;
        });

        ++done;
    };

    NFibers::Go(pool, init);

    pool.WaitIdle();

    ASSERT_EQ(done.load(), 2u);
}

TEST(TestFiber, RunInParallel) {
    NExecutors::IntrusiveThreadPool pool{3};
    pool.Start();

    std::atomic<size_t> completed{0};

    auto sleeper = [&]() {
        std::this_thread::sleep_for(3s);
        completed.fetch_add(1);
    };

    const auto start = Now();

    NFibers::Go(pool, sleeper);
    NFibers::Go(pool, sleeper);
    NFibers::Go(pool, sleeper);

    pool.WaitIdle();

    ASSERT_EQ(completed.load(), 3u);
    ASSERT_TRUE(Now() - start < 3s + 500ms);
}

TEST(TestFiber, Yield1) {
    NExecutors::IntrusiveThreadPool pool{1};
    pool.Start();

    bool done = false;

    NFibers::Go(pool, [&] {
        NFibers::Yield();

        ASSERT_EQ(NExecutors::IntrusiveThreadPool::Current(), &pool);
        done = true;
    });

    pool.WaitIdle();
    ASSERT_TRUE(done);
}

TEST(TestFiber, Yield2) {
    NExecutors::IntrusiveThreadPool pool{1};

    enum State : int { Ping = 0, Pong = 1 };

    int state = Ping;

    NFibers::Go(pool, [&] {
        for (size_t i = 0; i < 2; ++i) {
            ASSERT_EQ(state, Ping);
            state = Pong;

            NFibers::Yield();
        }
    });

    NFibers::Go(pool, [&] {
        for (size_t j = 0; j < 2; ++j) {
            ASSERT_EQ(state, Pong);
            state = Ping;

            NFibers::Yield();
        }
    });

    pool.Start();

    pool.WaitIdle();
}

TEST(TestFiber, Yield3) {
    NExecutors::IntrusiveThreadPool pool{4};

    static const size_t kYields = 1024;

    auto runner = [] {
        for (size_t i = 0; i < kYields; ++i) {
            NFibers::Yield();
        }
    };

    NFibers::Go(pool, runner);
    NFibers::Go(pool, runner);

    pool.Start();

    pool.WaitIdle();
}

TEST(TestFiber, TwoPools1) {
    NExecutors::IntrusiveThreadPool pool_1{4};
    NExecutors::IntrusiveThreadPool pool_2{4};

    pool_1.Start();
    pool_2.Start();

    auto make_tester = [](NExecutors::IntrusiveThreadPool& pool) {
        return
            [&pool]() { ASSERT_EQ(NExecutors::IntrusiveThreadPool::Current(), &pool); };
    };

    NFibers::Go(pool_1, make_tester(pool_1));
    NFibers::Go(pool_2, make_tester(pool_2));

    pool_1.WaitIdle();
    pool_2.WaitIdle();
}

TEST(TestFiber, TwoPools2) {
    NExecutors::IntrusiveThreadPool pool_1{4};
    pool_1.Start();

    NExecutors::IntrusiveThreadPool pool_2{4};
    pool_2.Start();

    auto make_tester = [](NExecutors::IntrusiveThreadPool& pool) {
        return [&pool]() {
            static const size_t kIterations = 128;

            for (size_t i = 0; i < kIterations; ++i) {
                ASSERT_EQ(NExecutors::IntrusiveThreadPool::Current(), &pool);

                NFibers::Yield();

                NFibers::Go(pool, [&pool]() {
                    ASSERT_EQ(NExecutors::IntrusiveThreadPool::Current(), &pool);
                });
            }
        };
    };

    NFibers::Go(pool_1, make_tester(pool_1));
    NFibers::Go(pool_2, make_tester(pool_2));

    pool_1.WaitIdle();
    pool_2.WaitIdle();
}