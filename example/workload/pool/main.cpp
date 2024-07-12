//
// Created by konstantin on 09.06.24.
//

#include <iostream>
#include <cassert>
#include <chrono>

#include <executor/pool/distributed_pool.h>
#include <executor/pool/intrusive_pool.h>
#include <fiber/sync/async_mutex.h>
#include <fiber/sync/wait_group.h>
#include <go/go.h>

using namespace std::chrono_literals;

const size_t CountThreads = std::thread::hardware_concurrency();

int main() {
//    NExecutors::IntrusiveThreadPool pool{3};
//    pool.Start();

    NExecutors::DistributedPool pool{CountThreads};

    static const size_t kTasks = 100'000;

    std::cout << "Start" << std::endl;
    pool.Start();

    size_t counter{};

    NFibers::Go(pool, [&counter] {
        NFibers::AsyncMutex mutex;
        NFibers::WaitGroup wg;

        wg.Add(kTasks);

        for (size_t i{}; i < kTasks; i++) {
            NFibers::Go([&] {
                {
                    std::lock_guard lock(mutex);
                    counter++;
                }
                wg.Done();
            });
        }

        wg.Wait();
        assert(counter == kTasks);
    });

    pool.WaitShutdown();
    return 0;
}