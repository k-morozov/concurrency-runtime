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

void AssertRunningOn(NExecutors::IntrusiveThreadPool& pool) {
    ASSERT_TRUE(NExecutors::IntrusiveThreadPool::Current() == &pool);
}

class Robot {
public:
    explicit Robot(NExecutors::IExecutor& executor) : strand_(executor) {}

    void Push() {
        NExecutors::Submit(strand_, [this] { ++steps_; });
    }

    size_t Steps() const { return steps_; }

private:
    NExecutors::IntrusiveStrand strand_;
    size_t steps_{0};
};

}  // namespace

TEST(TestStrand, JustWorks) {
    NExecutors::IntrusiveThreadPool pool{4};
    pool.Start();

    NExecutors::IntrusiveStrand strand{pool};

    bool done = false;

    NExecutors::Submit(strand, [&] { done = true; });

    pool.WaitIdle();

    ASSERT_TRUE(done);
}

TEST(TestStrand, Decorator) {
    NExecutors::IntrusiveThreadPool pool{4};
    pool.Start();

    NExecutors::IntrusiveStrand strand{pool};

    bool done{false};

    for (size_t i = 0; i < 128; ++i) {
        NExecutors::Submit(strand, [&] {
            AssertRunningOn(pool);
            done = true;
        });
    }

    pool.WaitIdle();

    ASSERT_TRUE(done);
}

TEST(TestStrand, Counter) {
    NExecutors::IntrusiveThreadPool pool{13};
    pool.Start();

    size_t counter = 0;

    NExecutors::IntrusiveStrand strand{pool};

    static const size_t kIncrements = 1234;

    for (size_t i = 0; i < kIncrements; ++i) {
        NExecutors::Submit(strand, [&] {
            AssertRunningOn(pool);
            ++counter;
        });
    };

    pool.WaitIdle();

    ASSERT_EQ(counter, kIncrements);
}

TEST(TestStrand, Fifo) {
    NExecutors::IntrusiveThreadPool pool{13};
    pool.Start();

    NExecutors::IntrusiveStrand strand{pool};

    size_t done = 0;

    static const size_t kTasks = 12345;

    for (size_t i = 0; i < kTasks; ++i) {
        NExecutors::Submit(strand, [&, i] {
            AssertRunningOn(pool);
            ASSERT_EQ(done++, i);
        });
    };

    pool.WaitIdle();

    ASSERT_EQ(done, kTasks);
}

TEST(TestStrand, ConcurrentStrands) {
    NExecutors::IntrusiveThreadPool pool{16};
    pool.Start();

    static const size_t kStrands = 128;

    std::deque<Robot> robots;
    for (size_t i = 0; i < kStrands; ++i) {
        robots.emplace_back(pool);
    }

    static const size_t kPushes = 64;
    static const size_t kIterations = 128;

    for (size_t iteration = 0; iteration < kIterations; ++iteration) {
        for (size_t strand = 0; strand < kStrands; ++strand) {
            for (size_t push = 0; push < kPushes; ++push) {
                robots[strand].Push();
            }
        }
    }

    pool.WaitIdle();

    for (size_t i = 0; i < kStrands; ++i) {
        ASSERT_EQ(robots[i].Steps(), kPushes * kIterations);
    }
}

TEST(TestStrand, ConcurrentSubmits) {
    NExecutors::IntrusiveThreadPool workers{2};
    NExecutors::IntrusiveStrand strand{workers};

    NExecutors::IntrusiveThreadPool clients{4};

    workers.Start();
    clients.Start();

    static const size_t kTasks = 1024;

    size_t task_count{0};

    for (size_t i = 0; i < kTasks; ++i) {
        NExecutors::Submit(clients, [&] {
            NExecutors::Submit(strand, [&] {
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
    NExecutors::IntrusiveManualExecutor manual;
    NExecutors::IntrusiveStrand strand{manual};

    static const size_t kTasks = 117;

    size_t tasks = 0;

    for (size_t i = 0; i < kTasks; ++i) {
        NExecutors::Submit(strand, [&] { ++tasks; });
    }

    manual.Drain();

    ASSERT_EQ(tasks, kTasks);
}

TEST(TestStrand, Batching) {
    NExecutors::IntrusiveManualExecutor manual;
    NExecutors::IntrusiveStrand strand{manual};

    static const size_t kTasks = 100;

    size_t completed = 0;
    for (size_t i = 0; i < kTasks; ++i) {
        NExecutors::Submit(strand, [&completed] { ++completed; });
    };

    size_t tasks = manual.Drain();
    ASSERT_LT(tasks, 5u);
}

TEST(TestStrand, StrandOverStrand) {
    NExecutors::IntrusiveThreadPool pool{4};
    pool.Start();

    NExecutors::IntrusiveStrand strand_1{pool};
    NExecutors::IntrusiveStrand strand_2{(NExecutors::IExecutor&)strand_1};

    static const size_t kTasks = 17;

    size_t tasks = 0;

    for (size_t i = 0; i < kTasks; ++i) {
        NExecutors::Submit(strand_2, [&tasks] { ++tasks; });
    }

    pool.WaitIdle();

    ASSERT_EQ(tasks, kTasks);
}

TEST(TestStrand, DoNotOccupyThread) {
    NExecutors::IntrusiveThreadPool pool{1};
    pool.Start();

    NExecutors::IntrusiveStrand strand{pool};

    NExecutors::Submit(pool, [] { std::this_thread::sleep_for(1s); });

    std::atomic<bool> stop_requested{false};

    auto snooze = []() { std::this_thread::sleep_for(10ms); };

    for (size_t i = 0; i < 100; ++i) {
        NExecutors::Submit(strand, snooze);
    }

    NExecutors::Submit(pool, [&stop_requested] { stop_requested.store(true); });

    while (!stop_requested.load()) {
        NExecutors::Submit(strand, snooze);
        std::this_thread::sleep_for(10ms);
    }

    pool.WaitIdle();
}

TEST(TestStrand, NonBlockingSubmit) {
    NExecutors::IntrusiveThreadPool pool{1};
    NExecutors::IntrusiveStrand strand{pool};

    pool.Start();

    NExecutors::Submit(strand, [&] {
        // Bubble
        std::this_thread::sleep_for(3s);
    });

    std::this_thread::sleep_for(256ms);

    {
        common::StopWatch stop_watch;
        NExecutors::Submit(strand, [&] {
            // Do nothing
        });

        ASSERT_LE(stop_watch.Elapsed(), 100ms);
    }

    pool.WaitIdle();
}