//
// Created by konstantin on 17.06.24.
//

#include "gtest/gtest.h"

#include <chrono>
#include <memory>

#include <common/clock.h>
#include <executor/manual/intrusive_manual_executor.h>
#include <executor/pool/intrusive_pool.h>
#include <executor/strand/intrusive_strand.h>
#include <executor/submit.h>

using namespace std::chrono_literals;

namespace {

void AssertRunningOn(executors::IntrusiveThreadPool& pool) {
    ASSERT_TRUE(executors::IntrusiveThreadPool::Current() == &pool);
}

class Robot {
public:
    explicit Robot(executors::IExecutor& executor) : strand_(executor) {}

    void Push() {
        executors::Submit(strand_, [this] { ++steps_; });
    }

    size_t Steps() const { return steps_; }

private:
    executors::IntrusiveStrand strand_;
    size_t steps_{0};
};

}  // namespace

TEST(TestStrand, JustWorks) {
    executors::IntrusiveThreadPool pool{4};
    pool.Start();

    executors::IntrusiveStrand strand{pool};

    bool done = false;

    executors::Submit(strand, [&] { done = true; });

    pool.WaitIdle();

    ASSERT_TRUE(done);
}

TEST(TestStrand, Decorator) {
    executors::IntrusiveThreadPool pool{4};
    pool.Start();

    executors::IntrusiveStrand strand{pool};

    bool done{false};

    for (size_t i = 0; i < 128; ++i) {
        executors::Submit(strand, [&] {
            AssertRunningOn(pool);
            done = true;
        });
    }

    pool.WaitIdle();

    ASSERT_TRUE(done);
}

TEST(TestStrand, Counter) {
    executors::IntrusiveThreadPool pool{13};
    pool.Start();

    size_t counter = 0;

    executors::IntrusiveStrand strand{pool};

    static const size_t kIncrements = 1234;

    for (size_t i = 0; i < kIncrements; ++i) {
        executors::Submit(strand, [&] {
            AssertRunningOn(pool);
            ++counter;
        });
    };

    pool.WaitIdle();

    ASSERT_EQ(counter, kIncrements);
}

TEST(TestStrand, Fifo) {
    executors::IntrusiveThreadPool pool{13};
    pool.Start();

    executors::IntrusiveStrand strand{pool};

    size_t done = 0;

    static const size_t kTasks = 12345;

    for (size_t i = 0; i < kTasks; ++i) {
        executors::Submit(strand, [&, i] {
            AssertRunningOn(pool);
            ASSERT_EQ(done++, i);
        });
    };

    pool.WaitIdle();

    ASSERT_EQ(done, kTasks);
}

TEST(TestStrand, ConcurrentStrands) {
    executors::IntrusiveThreadPool pool{16};
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
    executors::IntrusiveThreadPool workers{2};
    executors::IntrusiveStrand strand{workers};

    executors::IntrusiveThreadPool clients{4};

    workers.Start();
    clients.Start();

    static const size_t kTasks = 1024;

    size_t task_count{0};

    for (size_t i = 0; i < kTasks; ++i) {
        executors::Submit(clients, [&] {
            executors::Submit(strand, [&] {
                AssertRunningOn(workers);
                ++task_count;
            });
        });
    }

    clients.WaitIdle();
    workers.WaitIdle();

    ASSERT_EQ(task_count, kTasks);
}

TEST(TestStrand, StrandOverManual) {
    executors::IntrusiveManualExecutor manual;
    executors::IntrusiveStrand strand{manual};

    static const size_t kTasks = 117;

    size_t tasks = 0;

    for (size_t i = 0; i < kTasks; ++i) {
        executors::Submit(strand, [&] { ++tasks; });
    }

    manual.Drain();

    ASSERT_EQ(tasks, kTasks);
}

TEST(TestStrand, Batching) {
    executors::IntrusiveManualExecutor manual;
    executors::IntrusiveStrand strand{manual};

    static const size_t kTasks = 100;

    size_t completed = 0;
    for (size_t i = 0; i < kTasks; ++i) {
        executors::Submit(strand, [&completed] { ++completed; });
    };

    size_t tasks = manual.Drain();
    ASSERT_LT(tasks, 5u);
}

TEST(TestStrand, StrandOverStrand) {
    executors::IntrusiveThreadPool pool{4};
    pool.Start();

    executors::IntrusiveStrand strand_1{pool};
    executors::IntrusiveStrand strand_2{(executors::IExecutor&)strand_1};

    static const size_t kTasks = 17;

    size_t tasks = 0;

    for (size_t i = 0; i < kTasks; ++i) {
        executors::Submit(strand_2, [&tasks] { ++tasks; });
    }

    pool.WaitIdle();

    ASSERT_EQ(tasks, kTasks);
}

TEST(TestStrand, DoNotOccupyThread) {
    executors::IntrusiveThreadPool pool{1};
    pool.Start();

    executors::IntrusiveStrand strand{pool};

    executors::Submit(pool, [] { std::this_thread::sleep_for(1s); });

    std::atomic<bool> stop_requested{false};

    auto snooze = []() { std::this_thread::sleep_for(10ms); };

    for (size_t i = 0; i < 100; ++i) {
        executors::Submit(strand, snooze);
    }

    executors::Submit(pool, [&stop_requested] { stop_requested.store(true); });

    while (!stop_requested.load()) {
        executors::Submit(strand, snooze);
        std::this_thread::sleep_for(10ms);
    }

    pool.WaitIdle();
}

TEST(TestStrand, NonBlockingSubmit) {
    executors::IntrusiveThreadPool pool{1};
    executors::IntrusiveStrand strand{pool};

    pool.Start();

    executors::Submit(strand, [&] {
        // Bubble
        std::this_thread::sleep_for(3s);
    });

    std::this_thread::sleep_for(256ms);

    {
        common::StopWatch stop_watch;
        executors::Submit(strand, [&] {
            // Do nothing
        });

        ASSERT_LE(stop_watch.Elapsed(), 100ms);
    }

    pool.WaitIdle();
}