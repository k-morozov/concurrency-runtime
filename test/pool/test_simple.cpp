//
// Created by konstantin on 09.05.24.
//

#include "gtest/gtest.h"

#include <chrono>
#include <memory>
#include <thread>

#include <pool/executor/executor.h>
#include <pool/task/task.h>

using namespace std::chrono_literals;

class TestSimple : public ::testing::Test {
public:
};

struct TestTask : NExe::Task {
    void Run() override {
        completed = true;
        ++counter;
    }

    ~TestTask() override {
        if (counter >= 2) {
            //            FAIL_CHECK_MT("Seems like task body was run multiple
            //            times")
        }
    }

    bool completed = false;
    int counter = 0;
};

template <size_t msec>
struct SlowTask : NExe::Task {
    void Run() override {
        std::this_thread::sleep_for(msec * 1ms);
        completed = true;
    }

    bool completed = false;
};

struct FailedTask : NExe::Task {
    void Run() override { throw std::logic_error{"Failed"}; }
};

TEST_F(TestSimple, Destructor) { auto _ = NExe::MakeThreadPool(); }

TEST_F(TestSimple, StartShutdown) {
    auto pool = NExe::MakeThreadPool();
    pool->StartShutdown();
}

TEST_F(TestSimple, StartTwiceAndWait) {
    auto pool = NExe::MakeThreadPool();
    pool->StartShutdown();
    pool->StartShutdown();
    pool->WaitShutdown();
}

TEST_F(TestSimple, RunSingleTask) {
    auto pool = NExe::MakeThreadPool();
    auto task = std::make_shared<TestTask>();

    pool->Submit(task);
    task->Wait();

    ASSERT_TRUE(task->completed);
    ASSERT_TRUE(task->IsFinished());
    ASSERT_TRUE(task->IsCompleted());
    ASSERT_FALSE(task->IsCanceled());
    ASSERT_FALSE(task->IsFailed());
}

TEST_F(TestSimple, RunSingleFailingTask) {
    auto pool = NExe::MakeThreadPool();
    auto task = std::make_shared<FailedTask>();

    pool->Submit(task);
    task->Wait();

    ASSERT_FALSE(task->IsCompleted());
    ASSERT_FALSE(task->IsCanceled());
    ASSERT_TRUE(task->IsFailed());

    [[maybe_unused]] auto error = task->GetError();
    //    CHECK_THROWS_AS_MT(std::rethrow_exception(error), std::logic_error);
}

TEST_F(TestSimple, CancelSingleTask) {
    auto pool = NExe::MakeThreadPool();
    auto task = std::make_shared<TestTask>();
    task->Cancel();
    task->Wait();

    ASSERT_FALSE(task->IsCompleted());
    ASSERT_TRUE(task->IsCanceled());
    ASSERT_FALSE(task->IsFailed());

    pool->Submit(task);
    task->Wait();

    ASSERT_FALSE(task->IsCompleted());
    ASSERT_TRUE(task->IsCanceled());
    ASSERT_FALSE(task->IsFailed());
}
