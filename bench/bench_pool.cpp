//
// Created by konstantin on 25.06.24.
//

#include <thread>
#include <vector>
#include <chrono>
#include <format>

#include <benchmark/benchmark.h>
#include <executor/pool/distributed_pool.h>
#include <executor/pool/intrusive_pool.h>
#include <fiber/sync/async_mutex.h>
#include <fiber/sync/wait_group.h>
#include <go/go.h>

using namespace std::chrono_literals;

const size_t CountThreads = std::thread::hardware_concurrency();
static constexpr size_t kTasks = 100'000;
static constexpr size_t CountIteration = 10;
static constexpr size_t CountRepetitions = 5;

void bench_logic(NExecutors::IExecutor& pool) {
    size_t counter{};

    NFibers::Go(pool, [&counter] {
        NFibers::AsyncMutex mutex;
        NFibers::WaitGroup wg;

        wg.Add(kTasks);

        for (size_t i{}; i < kTasks; i++) {
            NFibers::Go([&] {
                {
                    std::lock_guard lock(mutex);
//                    std::this_thread::sleep_for(5ms);
                    counter++;
                }
                wg.Done();
            });
        }

        wg.Wait();
        assert(counter == kTasks);
    });
}

static void IntrusiveThreadPool(benchmark::State& state) {
    for (auto _ : state) {
        NExecutors::IntrusiveThreadPool pool{CountThreads};
        pool.Start();
        bench_logic(pool);
        pool.WaitIdle();
    }
}
BENCHMARK(IntrusiveThreadPool)
    ->Name(std::format("IntrusiveThreadPool_task_{}", kTasks))
    ->Repetitions(CountRepetitions)
    ->Iterations(CountIteration)
    ->Unit(benchmark::kMillisecond);

static void DistributedPool(benchmark::State& state) {
    for (auto _ : state) {
        NExecutors::DistributedPool pool{CountThreads};
        pool.Start();
        bench_logic(pool);
        pool.WaitIdle();
    }
}
BENCHMARK(DistributedPool)
    ->Name(std::format("DistributedPool_task_{}", kTasks))
    ->Repetitions(CountRepetitions)
    ->Iterations(CountIteration)
    ->Unit(benchmark::kMillisecond);