//
// Created by konstantin on 26.07.24.
//

#include <chrono>
#include <coroutine>
#include <condition_variable>
#include <latch>
#include <thread>
#include <vector>

#include <benchmark/benchmark.h>
#include <components/async_mutex/async_mutex.h>

using namespace std::chrono_literals;

static constexpr size_t CountIteration = 5;
static constexpr size_t CountRepetitions = 10;
static constexpr size_t MaxCountThreads = 8;
static constexpr size_t CountIterationsInThread = 100'00;

struct TestAsyncMutex final {
    NComponents::AsyncMutex mutex;
    size_t number{};

    std::latch latch;
    const size_t count_iterations;

    explicit TestAsyncMutex(size_t n, size_t count_iterations)
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

struct TestStdMutex final {
    std::mutex mutex;
    size_t number{};

    std::latch latch;
    const size_t count_iterations;

    explicit TestStdMutex(size_t n, size_t count_iterations)
        : latch(n), count_iterations(count_iterations) {}

    void run() {
        {
            std::unique_lock lock(cv_wait);
            while (!wait_flag) cv.wait(lock);
        }
        for (size_t i = 0; i < count_iterations; i++) {
            mutex.lock();
            number += 1;
            mutex.unlock();

            mutex.lock();
            number += 1;
            mutex.unlock();
        }

        latch.count_down();
    }

    void StartAll() {
        std::unique_lock lock(cv_wait);
        wait_flag.store(true);
        cv.notify_all();

        mutex.lock();
        mutex.unlock();
    }

    void Wait() { latch.wait(); }

private:
    std::mutex cv_wait;
    std::condition_variable cv;
    std::atomic<bool> wait_flag{};
};

static void BenchAsyncMutex(benchmark::State& state) {
    for (auto _ : state) {


        TestAsyncMutex worker(MaxCountThreads, CountIterationsInThread);
        {
            std::vector<std::jthread> workers;
            for (size_t i = 0; i < MaxCountThreads; i++) {
                workers.emplace_back(&TestAsyncMutex::run, &worker);
            }

            worker.StartAll();
            worker.Wait();
        }
    }
}
BENCHMARK(BenchAsyncMutex)
    ->Name("AsyncMutex")
    ->Repetitions(CountRepetitions)
    ->Iterations(CountIteration)
    ->Unit(benchmark::kMillisecond);

static void BenchStdMutex(benchmark::State& state) {
    for (auto _ : state) {
        TestStdMutex worker(MaxCountThreads, CountIterationsInThread);
        {
            std::vector<std::jthread> workers;
            for (size_t i = 0; i < MaxCountThreads; i++) {
                workers.emplace_back(&TestStdMutex::run, &worker);
            }

            worker.StartAll();
            worker.Wait();
        }
    }
}
BENCHMARK(BenchStdMutex)
    ->Name("StdMutex")
    ->Repetitions(CountRepetitions)
    ->Iterations(CountIteration)
    ->Unit(benchmark::kMillisecond);