//
// Created by konstantin on 28.05.24.
//

#include "gtest/gtest.h"

#include <thread>

#include <coro/coro.h>

using namespace NFibers::coro;

struct Threads {
    template <typename F>
    void Run(F task) {
        std::thread t([task = std::move(task)]() mutable { task(); });
        t.join();
    }
};

TEST(TestCoro, Threads) {
    size_t steps = 0;

    Coroutine co([&steps] {
        std::cout << "Step" << std::endl;
        ++steps;
        Coroutine::Suspend();
        std::cout << "Step" << std::endl;
        ++steps;
        Coroutine::Suspend();
        std::cout << "Step" << std::endl;
        ++steps;
    });

    auto resume = [&co] { co.Resume(); };

    // Simulate fiber running in thread pool
    Threads threads;
    threads.Run(resume);
    threads.Run(resume);
    threads.Run(resume);

    ASSERT_EQ(steps, 3u);
}

TEST(TestCoro, Pipeline) {
    const size_t kSteps = 123;

    size_t step_count = 0;

    Coroutine a([&] {
        Coroutine b([&] {
            for (size_t i = 0; i < kSteps; ++i) {
                ++step_count;
                Coroutine::Suspend();
            }
        });

        while (!b.IsCompleted()) {
            b.Resume();
            Coroutine::Suspend();
        }
    });

    while (!a.IsCompleted()) {
        a.Resume();
    }

    ASSERT_EQ(step_count, kSteps);
}