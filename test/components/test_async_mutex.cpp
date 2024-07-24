//
// Created by konstantin on 20.07.24.
//

#include "gtest/gtest.h"

#include <condition_variable>
#include <thread>

#include <components/async_mutex/async_mutex.h>

using namespace std::chrono_literals;

namespace {

struct TestWork {
    NComponents::AsyncMutex mutex;
    size_t number{};

    NComponents::ResumableNoOwn run() {
        {
            std::unique_lock lock(cv_wait);
            while (!wait_flag) cv.wait(lock);
        }
        co_await mutex.lock();
        number += 1;
        mutex.unlock();
    }

    void StartAll() {
        std::unique_lock lock(cv_wait);
        wait_flag.store(true);
        cv.notify_all();
    }

private:
    std::mutex cv_wait;
    std::condition_variable cv;
    std::atomic<bool> wait_flag{};
};

}  // namespace

TEST(TestAsyncMutex, JustWorking) {
    NComponents::AsyncMutex mutex;

    mutex.lock();
    mutex.unlock();
}

TEST(TestAsyncMutex, SomeThreads) {
    TestWork worker;

    constexpr size_t MaxCount = 32;
    {
        std::vector<std::jthread> workers;
        for (size_t i = 0; i < MaxCount; i++) {
            workers.emplace_back(&TestWork::run, &worker);
        }

        worker.StartAll();
    }

    ASSERT_EQ(worker.number, MaxCount);
}