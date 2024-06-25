//
// Created by konstantin on 25.06.24.
//

#include "gtest/gtest.h"

#include <memory>
#include <thread>

#include <executor/task/task.h>
#include <executor/pool/distributed_pool.h>
#include <executor/submit.h>
#include <fiber/sync/wait_group.h>

using namespace std::chrono_literals;

TEST(TestDistributedPool, JustWorks) {
    NExecutors::DistributedPool pool{4};

//    pool.Start();
//
//    fibers::WaitGroup wg;
//    wg.Add(1);
//
//    NExecutors::Submit(pool, [&wg] {
//        std::cout << "Hello from thread pool!" << std::endl;
//        wg.Done();
//    });
//
//    wg.Wait();
}