//
// Created by konstantin on 20.07.24.
//

#include "gtest/gtest.h"

#include <condition_variable>
#include <latch>
#include <thread>

#include <components/async_mutex/async_mutex.h>

using namespace std::chrono_literals;

namespace {

struct TestJust final {
    NComponents::AsyncMutex mutex;

    NComponents::ResumableNoOwn run() {
        co_await mutex.lock();
        mutex.unlock();
    }
};

struct TestSyncIncrement final {
    NComponents::AsyncMutex mutex;
    size_t number{};

    std::latch latch;
    const size_t count_iterations;

    explicit TestSyncIncrement(size_t n, size_t count_iterations)
        : latch(n), count_iterations(count_iterations) {}

    NComponents::ResumableNoOwn run() {
        {
            std::unique_lock lock(cv_wait);
            while (!wait_flag) cv.wait(lock);
        }
        for (size_t i = 0; i < count_iterations; i++) {
            co_await mutex.lock();
            number += 1;
            mutex.unlock();

            co_await mutex.lock();
            number += 1;
            mutex.unlock();
        }

        latch.count_down();
    }

    NComponents::ResumableNoOwn StartAll() {
        std::unique_lock lock(cv_wait);
        wait_flag.store(true);
        cv.notify_all();

        co_await mutex.lock();
        mutex.unlock();
    }

    void Wait() { latch.wait(); }

private:
    std::mutex cv_wait;
    std::condition_variable cv;
    std::atomic<bool> wait_flag{};
};

struct TestWait final {
    NComponents::AsyncMutex mutex;
    bool status{};

    NComponents::ResumableNoOwn run() {
        co_await mutex.lock();

        std::jthread th(&TestWait::internal_run, this);

        std::this_thread::sleep_for(5s);

        if (status) throw std::logic_error("no wait");

        mutex.unlock();

        if (!status) throw std::logic_error("status is false");
    }

private:
    NComponents::ResumableNoOwn internal_run() {
        co_await mutex.lock();
        status = true;
        mutex.unlock();
    }
};

}  // namespace

TEST(TestAsyncMutex, JustWorking) {
    TestJust worker;

    std::jthread th(&TestJust::run, &worker);
}

TEST(TestAsyncMutex, SyncIncrementInThreads) {
    constexpr size_t MaxCountThreads = 32;
    constexpr size_t CountIterations = 100;

    TestSyncIncrement worker(MaxCountThreads, CountIterations);
    {
        std::vector<std::jthread> workers;
        for (size_t i = 0; i < MaxCountThreads; i++) {
            workers.emplace_back(&TestSyncIncrement::run, &worker);
        }

        worker.StartAll();
        worker.Wait();
    }

    ASSERT_EQ(worker.number, 2 * MaxCountThreads * CountIterations);
}

TEST(TestAsyncMutex, ThreadWait) {
    TestWait worker;

    std::jthread th(&TestWait::run, &worker);
}