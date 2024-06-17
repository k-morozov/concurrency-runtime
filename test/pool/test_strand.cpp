//
// Created by konstantin on 17.06.24.
//

#include "gtest/gtest.h"

#include <chrono>
#include <memory>

#include <executor/manual/manual_executor.h>
#include <executor/pool/thread_pool.h>
#include <executor/strand/strand.h>
#include <executor/submit.h>
#include <executor/task/fiber_task.h>

using namespace std::chrono_literals;

namespace {

void AssertRunningOn(executors::ThreadPool& pool) {
    ASSERT_TRUE(executors::ThreadPool::Current() == &pool);
}

class Robot {
public:
    explicit Robot(executors::IExecutor& executor) : strand_(executor) {}

    void Push() {
        executors::Submit(strand_, std::make_shared<executors::FiberTask>(
                                       [this] { ++steps_; }));
    }

    size_t Steps() const { return steps_; }

private:
    executors::Strand strand_;
    size_t steps_{0};
};

}  // namespace

TEST(TestStrand, JustWorks) {
    executors::ThreadPool pool{4};
    pool.Start();

    executors::Strand strand{pool};

    bool done = false;

    executors::Submit(
        strand, std::make_shared<executors::FiberTask>([&] { done = true; }));

    pool.WaitIdle();

    ASSERT_TRUE(done);
}

TEST(TestStrand, Decorator) {
    executors::ThreadPool pool{4};
    pool.Start();

    executors::Strand strand{pool};

    bool done{false};

    for (size_t i = 0; i < 128; ++i) {
        executors::Submit(strand, std::make_shared<executors::FiberTask>([&] {
                              AssertRunningOn(pool);
                              done = true;
                          }));
    }

    pool.WaitIdle();

    ASSERT_TRUE(done);
}

TEST(TestStrand, Counter) {
    executors::ThreadPool pool{13};
    pool.Start();

    size_t counter = 0;

    executors::Strand strand{pool};

    static const size_t kIncrements = 1234;

    for (size_t i = 0; i < kIncrements; ++i) {
        executors::Submit(strand, std::make_shared<executors::FiberTask>([&] {
                              AssertRunningOn(pool);
                              ++counter;
                          }));
    };

    pool.WaitIdle();

    ASSERT_EQ(counter, kIncrements);
}

TEST(TestStrand, Fifo) {
    executors::ThreadPool pool{13};
    pool.Start();

    executors::Strand strand{pool};

    size_t done = 0;

    static const size_t kTasks = 12345;

    for (size_t i = 0; i < kTasks; ++i) {
        executors::Submit(strand,
                          std::make_shared<executors::FiberTask>([&, i] {
                              AssertRunningOn(pool);
                              ASSERT_EQ(done++, i);
                          }));
    };

    pool.WaitIdle();

    ASSERT_EQ(done, kTasks);
}

TEST(TestStrand, ConcurrentStrands) {
    executors::ThreadPool pool{16};
    pool.Start();

    static const size_t kStrands = 50;

    std::deque<Robot> robots;
    for (size_t i = 0; i < kStrands; ++i) {
        robots.emplace_back(pool);
    }

    static const size_t kPushes = 25;
    static const size_t kIterations = 25;

    for (size_t i = 0; i < kIterations; ++i) {
        for (size_t j = 0; j < kStrands; ++j) {
            for (size_t k = 0; k < kPushes; ++k) {
                robots[j].Push();
            }
        }
    }

    pool.WaitIdle();

    for (size_t i = 0; i < kStrands; ++i) {
        ASSERT_EQ(robots[i].Steps(), kPushes * kIterations);
    }
}

TEST(TestStrand, ConcurrentSubmits) {
    executors::ThreadPool workers{2};
    executors::Strand strand{workers};

    executors::ThreadPool clients{4};

    workers.Start();
    clients.Start();

    static const size_t kTasks = 1024;

    size_t task_count{0};

    for (size_t i = 0; i < kTasks; ++i) {
        executors::Submit(clients, std::make_shared<executors::FiberTask>([&] {
                              executors::Submit(
                                  strand,
                                  std::make_shared<executors::FiberTask>([&] {
                                      AssertRunningOn(workers);
                                      ++task_count;
                                  }));
                          }));
    }

    clients.WaitIdle();
    workers.WaitIdle();

    ASSERT_EQ(task_count, kTasks);
}

TEST(TestStrand, StrandOverManual) {
    executors::ManualExecutor manual;
    executors::Strand strand{manual};

    static const size_t kTasks = 117;

    size_t tasks = 0;

    for (size_t i = 0; i < kTasks; ++i) {
        executors::Submit(
            strand, std::make_shared<executors::FiberTask>([&] { ++tasks; }));
    }

    manual.Drain();

    ASSERT_EQ(tasks, kTasks);
}

TEST(TestStrand, Batching) {
    executors::ManualExecutor manual;
    executors::Strand strand{manual};

    static const size_t kTasks = 100;

    size_t completed = 0;
    for (size_t i = 0; i < kTasks; ++i) {
        executors::Submit(strand, std::make_shared<executors::FiberTask>(
                                      [&completed] { ++completed; }));
    };

    size_t tasks = manual.Drain();
    ASSERT_LT(tasks, 5u);
}

TEST(TestStrand, StrandOverStrand) {
    executors::ThreadPool pool{4};
    pool.Start();

    executors::Strand strand_1{pool};
    executors::Strand strand_2{(executors::IExecutor&)strand_1};

    static const size_t kTasks = 17;

    size_t tasks = 0;

    for (size_t i = 0; i < kTasks; ++i) {
        executors::Submit(strand_2, std::make_shared<executors::FiberTask>(
                                        [&tasks] { ++tasks; }));
    }

    pool.WaitIdle();

    ASSERT_EQ(tasks, kTasks);
}

TEST(TestStrand, DoNotOccupyThread) {
    executors::ThreadPool pool{1};
    pool.Start();

    executors::Strand strand{pool};

    executors::Submit(pool, std::make_shared<executors::FiberTask>(
                                [] { std::this_thread::sleep_for(1s); }));

    std::atomic<bool> stop_requested{false};

    auto snooze = []() { std::this_thread::sleep_for(10ms); };

    for (size_t i = 0; i < 100; ++i) {
        executors::Submit(strand,
                          std::make_shared<executors::FiberTask>(snooze));
    }

    executors::Submit(pool,
                      std::make_shared<executors::FiberTask>(
                          [&stop_requested] { stop_requested.store(true); }));

    while (!stop_requested.load()) {
        executors::Submit(strand,
                          std::make_shared<executors::FiberTask>(snooze));
        std::this_thread::sleep_for(10ms);
    }

    pool.WaitIdle();
}

TEST(TestStrand, NonBlockingSubmit) {
    executors::ThreadPool pool{1};
    executors::Strand strand{pool};

    pool.Start();

    executors::Submit(strand, std::make_shared<executors::FiberTask>([&] {
                          // Bubble
                          std::this_thread::sleep_for(3s);
                      }));

    std::this_thread::sleep_for(256ms);

    {
        //        wheels::StopWatch stop_watch;
        executors::Submit(strand, std::make_shared<executors::FiberTask>([&] {
                              // Do nothing
                          }));

        //        ASSERT_LE(stop_watch.Elapsed(), 100ms);
    }

    pool.WaitIdle();
}