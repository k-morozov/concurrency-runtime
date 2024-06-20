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

class TestFiber : public ::testing::Test {
protected:
    std::unique_ptr<executors::IntrusiveThreadPool> pool;

    void SetUp() override {
//        int num_threads = GetParam();
        pool = std::make_unique<executors::IntrusiveThreadPool>(3);
        pool->Start();
    }

    void TearDown() override {
        pool.reset();
    }
};

//using PoolTypes = ::testing::Types<executors::ThreadPool, executors::IntrusiveThreadPool>;
//
//TYPED_TEST_SUITE(TestFiber, PoolTypes);

class TableParametrizedFixture :
    public TestFiber,
    public ::testing::WithParamInterface<std::tuple<int, int>>
{};

struct Params {
    size_t insert_count;
    size_t get_count;
};

std::tuple<size_t, size_t> ConvertFromParam(const Params& p) {
    return std::make_tuple(p.insert_count, p.get_count);
}

INSTANTIATE_TEST_SUITE_P(
    BasicInsertion,
    TableParametrizedFixture,
    ::testing::Values(
        ConvertFromParam({
            .insert_count=10,
            .get_count=10}),
        ConvertFromParam({
            .insert_count=10,
            .get_count=100}),
        ConvertFromParam({
            .insert_count=100,
            .get_count=10}),
        ConvertFromParam({
            .insert_count=10,
            .get_count=10'000}),
        ConvertFromParam({
            .insert_count=10'000,
            .get_count=10})
        ));

struct ExpectedRow {
    size_t row_id;
    size_t row;
};

TEST_P(TableParametrizedFixture, JustWorks1Go) {
//    executors::ThreadPool pool{3};
//    executors::IntrusiveThreadPool pool{3};
    this->pool->Start();

    bool done = false;

    fibers::Go(*this->pool, [&]() {
//        ASSERT_EQ(executors::ThreadPool::Current(), &pool);
//        ASSERT_EQ(executors::IntrusiveThreadPool::Current(), &pool);
        done = true;
    });

    this->pool->WaitIdle();

    ASSERT_TRUE(done);
}

//TEST(TestFiber, Child) {
//    executors::ThreadPool pool{3};
//    pool.Start();
//
//    std::atomic<size_t> done{0};
//
//    auto init = [&]() {
//        ASSERT_EQ(executors::ThreadPool::Current(), &pool);
//
//        fibers::Go([&]() {
//            ASSERT_EQ(executors::ThreadPool::Current(), &pool);
//            ++done;
//        });
//
//        ++done;
//    };
//
//    fibers::Go(pool, init);
//
//    pool.WaitIdle();
//
//    ASSERT_EQ(done.load(), 2u);
//}
//
//TEST(TestFiber, RunInParallel) {
//    executors::ThreadPool pool{3};
//    pool.Start();
//
//    std::atomic<size_t> completed{0};
//
//    auto sleeper = [&]() {
//        std::this_thread::sleep_for(3s);
//        completed.fetch_add(1);
//    };
//
//    const auto start = Now();
//
//    fibers::Go(pool, sleeper);
//    fibers::Go(pool, sleeper);
//    fibers::Go(pool, sleeper);
//
//    pool.WaitIdle();
//
//    ASSERT_EQ(completed.load(), 3u);
//    ASSERT_TRUE(Now() - start < 3s + 500ms);
//}
//
//TEST(TestFiber, Yield1) {
//    executors::ThreadPool pool{1};
//    pool.Start();
//
//    bool done = false;
//
//    fibers::Go(pool, [&] {
//        fibers::Yield();
//
//        ASSERT_EQ(executors::ThreadPool::Current(), &pool);
//        done = true;
//    });
//
//    pool.WaitIdle();
//    ASSERT_TRUE(done);
//}
//
//TEST(TestFiber, Yield2) {
//    executors::ThreadPool pool{1};
//
//    enum State : int { Ping = 0, Pong = 1 };
//
//    int state = Ping;
//
//    fibers::Go(pool, [&] {
//        for (size_t i = 0; i < 2; ++i) {
//            ASSERT_EQ(state, Ping);
//            state = Pong;
//
//            fibers::Yield();
//        }
//    });
//
//    fibers::Go(pool, [&] {
//        for (size_t j = 0; j < 2; ++j) {
//            ASSERT_EQ(state, Pong);
//            state = Ping;
//
//            fibers::Yield();
//        }
//    });
//
//    pool.Start();
//
//    pool.WaitIdle();
//}
//TEST(TestFiber, Yield3) {
//    executors::ThreadPool pool{4};
//
//    static const size_t kYields = 1024;
//
//    auto runner = [] {
//        for (size_t i = 0; i < kYields; ++i) {
//            fibers::Yield();
//        }
//    };
//
//    fibers::Go(pool, runner);
//    fibers::Go(pool, runner);
//
//    pool.Start();
//
//    pool.WaitIdle();
//}
//
//TEST(TestFiber, TwoPools1) {
//    executors::ThreadPool pool_1{4};
//    executors::ThreadPool pool_2{4};
//
//    pool_1.Start();
//    pool_2.Start();
//
//    auto make_tester = [](executors::ThreadPool& pool) {
//        return
//            [&pool]() { ASSERT_EQ(executors::ThreadPool::Current(), &pool); };
//    };
//
//    fibers::Go(pool_1, make_tester(pool_1));
//    fibers::Go(pool_2, make_tester(pool_2));
//
//    pool_1.WaitIdle();
//    pool_2.WaitIdle();
//}
//
//TEST(TestFiber, TwoPools2) {
//    executors::ThreadPool pool_1{4};
//    pool_1.Start();
//
//    executors::ThreadPool pool_2{4};
//    pool_2.Start();
//
//    auto make_tester = [](executors::ThreadPool& pool) {
//        return [&pool]() {
//            static const size_t kIterations = 128;
//
//            for (size_t i = 0; i < kIterations; ++i) {
//                ASSERT_EQ(executors::ThreadPool::Current(), &pool);
//
//                fibers::Yield();
//
//                fibers::Go(pool, [&pool]() {
//                    ASSERT_EQ(executors::ThreadPool::Current(), &pool);
//                });
//            }
//        };
//    };
//
//    fibers::Go(pool_1, make_tester(pool_1));
//    fibers::Go(pool_2, make_tester(pool_2));
//
//    pool_1.WaitIdle();
//    pool_2.WaitIdle();
//}