//
// Created by konstantin on 20.07.24.
//

#include "gtest/gtest.h"

#include <condition_variable>
#include <thread>

#include <components/async_mutex/async_mutex.h>

using namespace std::chrono_literals;

// TEST(TestAsyncMutex, JustWorking) {
//     NComponents::AsyncMutex mutex;
//
//     mutex.lock();
//     mutex.unlock();
// }

NComponents::ResumableNoOwn consumer(NComponents::AsyncMutex& mutex,
                                     size_t& number, std::mutex& cv_wait,
                                     std::condition_variable& cv,
                                     std::atomic<bool>& wait_flag) {
    {
        std::unique_lock lock(cv_wait);
        while (!wait_flag) cv.wait(lock);
    }
    co_await mutex.lock();
    std::osyncstream(std::cout)
        << "[main][thead_id=" << std::this_thread::get_id()
        << "] increase number" << std::endl;
    number += 1;
    mutex.unlock();
}

TEST(TestAsyncMutex, SomeThreads) {
    NComponents::AsyncMutex mutex;
    size_t number{};

    std::mutex cv_wait;
    std::condition_variable cv;
    std::atomic<bool> wait_flag{};

    constexpr size_t MaxCount = 2;
    {
        std::vector<std::jthread> workers;
        for (size_t i = 0; i < MaxCount; i++) {
            // https://stackoverflow.com/questions/60592174/lambda-lifetime-explanation-for-c20-coroutines
            workers.emplace_back(consumer, std::ref(mutex), std::ref(number),
                                 std::ref(cv_wait), std::ref(cv),
                                 std::ref(wait_flag));
        }

        {
            std::unique_lock lock(cv_wait);
            wait_flag.store(true);
            cv.notify_all();
        }
    }

    ASSERT_EQ(number, MaxCount);
}