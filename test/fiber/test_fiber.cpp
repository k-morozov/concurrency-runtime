//
// Created by konstantin on 10.05.24.
//

#include "gtest/gtest.h"

#include <chrono>

#include <executor/pool/thread_pool.h>
#include <go/go.h>

using namespace std::chrono_literals;

namespace {
auto Now() { return std::chrono::high_resolution_clock::now(); }
}  // namespace

TEST(TestFiber, JustWorks1Go) {
    executors::ThreadPool pool{3};
    pool.Start();

    bool done = false;

    fibers::Go(pool, [&]() {
        ASSERT_EQ(executors::ThreadPool::Current(), &pool);
        done = true;
    });

    pool.WaitIdle();

    ASSERT_TRUE(done);
}

TEST(TestFiber, Child) {
    executors::ThreadPool pool{3};
    pool.Start();

    std::atomic<size_t> done{0};

    auto init = [&]() {
        ASSERT_EQ(executors::ThreadPool::Current(), &pool);

        fibers::Go([&]() {
            ASSERT_EQ(executors::ThreadPool::Current(), &pool);
            ++done;
        });

        ++done;
    };

    fibers::Go(pool, init);

    pool.WaitIdle();

    ASSERT_EQ(done.load(), 2u);
}

TEST(TestFiber, RunInParallel) {
    executors::ThreadPool pool{3};
    pool.Start();

    std::atomic<size_t> completed{0};

    auto sleeper = [&]() {
        std::this_thread::sleep_for(3s);
        completed.fetch_add(1);
    };

    const auto start = Now();

    fibers::Go(pool, sleeper);
    fibers::Go(pool, sleeper);
    fibers::Go(pool, sleeper);

    pool.WaitIdle();

    ASSERT_EQ(completed.load(), 3u);
    ASSERT_TRUE(Now() - start < 3s + 500ms);
}

TEST(TestFiber, Yield1) {
    executors::ThreadPool pool{1};
    pool.Start();

    bool done = false;

    fibers::Go(pool, [&] {
        fibers::Yield();

        ASSERT_EQ(executors::ThreadPool::Current(), &pool);
        done = true;
    });

    pool.WaitIdle();
    ASSERT_TRUE(done);
}

TEST(TestFiber, Yield2) {
    executors::ThreadPool pool{1};

    enum State : int { Ping = 0, Pong = 1 };

    int state = Ping;

    fibers::Go(pool, [&] {
        for (size_t i = 0; i < 2; ++i) {
            ASSERT_EQ(state, Ping);
            state = Pong;

            fibers::Yield();
        }
    });

    fibers::Go(pool, [&] {
        for (size_t j = 0; j < 2; ++j) {
            ASSERT_EQ(state, Pong);
            state = Ping;

            fibers::Yield();
        }
    });

    pool.Start();

    pool.WaitIdle();
}
TEST(TestFiber, Yield3) {
    executors::ThreadPool pool{4};

    static const size_t kYields = 1024;

    auto runner = [] {
        for (size_t i = 0; i < kYields; ++i) {
            fibers::Yield();
        }
    };

    fibers::Go(pool, runner);
    fibers::Go(pool, runner);

    pool.Start();

    pool.WaitIdle();
}

TEST(TestFiber, TwoPools1) {
    executors::ThreadPool pool_1{4};
    executors::ThreadPool pool_2{4};

    pool_1.Start();
    pool_2.Start();

    auto make_tester = [](executors::ThreadPool& pool) {
        return
            [&pool]() { ASSERT_EQ(executors::ThreadPool::Current(), &pool); };
    };

    fibers::Go(pool_1, make_tester(pool_1));
    fibers::Go(pool_2, make_tester(pool_2));

    pool_1.WaitIdle();
    pool_2.WaitIdle();
}

TEST(TestFiber, TwoPools2) {
    executors::ThreadPool pool_1{4};
    pool_1.Start();

    executors::ThreadPool pool_2{4};
    pool_2.Start();

    auto make_tester = [](executors::ThreadPool& pool) {
        return [&pool]() {
            static const size_t kIterations = 128;

            for (size_t i = 0; i < kIterations; ++i) {
                ASSERT_EQ(executors::ThreadPool::Current(), &pool);

                fibers::Yield();

                fibers::Go(pool, [&pool]() {
                    ASSERT_EQ(executors::ThreadPool::Current(), &pool);
                });
            }
        };
    };

    fibers::Go(pool_1, make_tester(pool_1));
    fibers::Go(pool_2, make_tester(pool_2));

    pool_1.WaitIdle();
    pool_2.WaitIdle();
}