//
// Created by konstantin on 09.06.24.
//

#include <iostream>

#include <executor/pool/intrusive_pool.h>
#include <fiber/sync/async_mutex.h>
#include <fiber/sync/wait_group.h>
#include <go/go.h>

int main() {
    NExecutors::IntrusiveThreadPool scheduler{4};
    scheduler.Start();

    constexpr size_t kFibers = 128;
    constexpr size_t kTasks = 1024;

    NFibers::Go(scheduler, [] {
        NFibers::WaitGroup wg;

        NFibers::AsyncMutex mutex;
        size_t cs = 0;

        wg.Add(kFibers);

        for (size_t i = 0; i < kFibers; ++i) {
            NFibers::Go([&] {
                for (size_t j = 0; j < kTasks; ++j) {
                    std::lock_guard guard(mutex);
                    ++cs;
                }
                wg.Done();
            });
        }

        wg.Wait();

        std::cout << "# critical sections: " << cs << std::endl;
    });

    scheduler.WaitIdle();
    return 0;
}