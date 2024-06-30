//
// Created by konstantin on 09.06.24.
//

#include <iostream>
#include <cassert>

#include <executor/pool/distributed_pool.h>
#include <executor/pool/intrusive_pool.h>
#include <fiber/sync/async_mutex.h>
#include <fiber/sync/wait_group.h>
#include <go/go.h>

int main() {
    NExecutors::IntrusiveThreadPool scheduler{4};
    scheduler.Start();

    NExecutors::DistributedPool pool{4};

    static const size_t kTasks = 1'000'000;

    pool.Start();

    size_t counter{};

    fibers::Go(pool, [&counter] {
        fibers::AsyncMutex mutex;
        fibers::WaitGroup wg;

        wg.Add(kTasks);

        for (size_t i{}; i < kTasks; i++) {
            fibers::Go([&] {
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
    return 0;
}