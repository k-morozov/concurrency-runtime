//
// Created by konstantin on 20.07.24.
//

#include "gtest/gtest.h"

#include <condition_variable>
#include <thread>
#include <latch>

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

    explicit TestSyncIncrement(size_t n) : latch(n) {}

    NComponents::ResumableNoOwn run() {
        {
            std::unique_lock lock(cv_wait);
            while (!wait_flag) cv.wait(lock);
        }
        co_await mutex.lock();
        number += 1;
        mutex.unlock();

        latch.count_down();
    }

    NComponents::ResumableNoOwn StartAll() {
        std::unique_lock lock(cv_wait);
        wait_flag.store(true);
        cv.notify_all();

        co_await mutex.lock();
        mutex.unlock();
    }

    void Wait() {
        latch.wait();
    }

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

        if (status)
            throw std::logic_error("no wait");

        mutex.unlock();

        if (!status)
            throw std::logic_error("status is false");
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
    constexpr size_t MaxCount = 2;

    TestSyncIncrement worker(MaxCount);
    {
        std::vector<std::jthread> workers;
        for (size_t i = 0; i < MaxCount; i++) {
            workers.emplace_back(&TestSyncIncrement::run, &worker);
        }

        worker.StartAll();
        worker.Wait();
    }

    ASSERT_EQ(worker.number, MaxCount);
}

//TEST(TestAsyncMutex, ThreadWait) {
//    TestWait worker;
//
//    std::jthread th(&TestWait::run, &worker);
//}