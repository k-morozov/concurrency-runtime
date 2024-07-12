//
// Created by konstantin on 25.06.24.
//

#include "gtest/gtest.h"

#include <thread>

#include <executor/pool/distributed_pool.h>
#include <executor/pool/intrusive_pool.h>
#include <fiber/sync/wait_group.h>
#include <go/go.h>
#include <fiber/sync/async_mutex.h>

#include "common/clock.h"

using namespace std::chrono_literals;

TEST(TestDistributedPool, JustWorks) {
    int a = 1;

    {
        NExecutors::DistributedPool pool{4};
        pool.Start();

        NFibers::Go(pool, [&a] {
            NFibers::WaitGroup wg;
            wg.Add(1);

            ASSERT_EQ(a, 1);
            a++;

            NFibers::Go([&wg, &a] {
                ASSERT_EQ(a, 2);
                a++;
                wg.Done();
            });
            wg.Wait();
        });

        pool.WaitIdle();
    }

    ASSERT_EQ(a, 3);
}

TEST(TestDistributedPool, DISABLED_JustWorks2) {
    NExecutors::DistributedPool pool{4};

    NFibers::WaitGroup wg;
    wg.Add(1);

    NFibers::Go(pool, [&wg] {
        std::cout << "Hello from thread pool! " << std::endl;
        wg.Done();
    });

    wg.Wait();
}

TEST(TestDistributedPool, Wait) {
    int a = 100;
    {
        NExecutors::DistributedPool pool{4};

        pool.Start();

        NFibers::Go(pool, [&a] {
            NFibers::WaitGroup wg;
            wg.Add(1);

            ASSERT_EQ(a, 100);
            a = 50;

            NFibers::Go([&] {
                std::this_thread::sleep_for(1s);
                a = 10;
                wg.Done();
            });

            wg.Wait();
            ASSERT_EQ(a, 10);
            a = 12;
        });

        pool.WaitIdle();
    }

    ASSERT_EQ(a, 12);
}

TEST(TestDistributedPool, MultiWait) {
    int a = 13;
    {
        NExecutors::DistributedPool pool{4};

        pool.Start();

        NFibers::Go(pool, [&] {
            for (size_t i = 0; i < 3; ++i) {
                NFibers::WaitGroup wg;
                wg.Add(1);

                NFibers::Go([&] {
                    std::this_thread::sleep_for(1s);
                    a = 113;
                    wg.Done();
                });

                wg.Wait();
            }
        });

        pool.WaitIdle();
    }
    ASSERT_EQ(a, 113);
}

TEST(TestDistributedPool, ManyTasks) {
    bool fully_completed = false;
    {
        NExecutors::DistributedPool pool{4};

        pool.Start();

        NFibers::Go(pool, [&] {
            static const size_t kTasks = 17;

            NFibers::WaitGroup wg;
            wg.Add(kTasks);

            std::atomic<size_t> go_completed{};
            for (size_t i = 0; i < kTasks; ++i) {
                NFibers::Go([&] {
                    go_completed++;
                    wg.Done();
                });
            }

            wg.Wait();
            std::cout << "go_completed=" << go_completed.load() << std::endl;
            ASSERT_EQ(go_completed.load(), kTasks);

            fully_completed = true;
        });
        pool.WaitIdle();
    }
    std::cout << "fully_completed=" << fully_completed << std::endl;
    ASSERT_TRUE(fully_completed);
}

TEST(TestDistributedPool, DoNotBurnCPU) {
    bool fully_completed = false;
    {
        NExecutors::DistributedPool pool{4};

        pool.Start();

        NFibers::Go(pool, [&] {
            NFibers::WaitGroup wg;
            wg.Add(4);

            common::ProcessCPUTimer cpu_timer;

            // Warmup
            for (size_t i = 0; i < 4; ++i) {
                NFibers::Go([&] {
                    std::this_thread::sleep_for(100ms);
                    wg.Done();
                });
            }

            wg.Wait();

            ASSERT_TRUE(cpu_timer.Spent() < 100ms);

            fully_completed = true;
        });

        pool.WaitIdle();
    }
    ASSERT_TRUE(fully_completed);
}

TEST(TestDistributedPool, Current) {
    bool fully_completed = false;
    {
        NExecutors::DistributedPool pool{4};

        pool.Start();

        NFibers::Go(pool, [&] {
            NFibers::WaitGroup wg;
            wg.Add(1);

            NFibers::Go([&] {
                ASSERT_EQ(NExecutors::DistributedPool::Current(), &pool);
                wg.Done();
            });

            wg.Wait();
            fully_completed = true;
        });

        pool.WaitIdle();
    }
    ASSERT_TRUE(fully_completed);
}

TEST(TestDistributedPool, SubmitAfterWait) {
    bool fully_completed = false;
    {
        NExecutors::DistributedPool pool{4};

        pool.Start();

        NFibers::Go(pool, [&] {
            NFibers::WaitGroup wg;
            wg.Add(1);

            NFibers::Go([&] {
                std::this_thread::sleep_for(500ms);

                NFibers::Go(*NExecutors::DistributedPool::Current(), [&] {
                    std::this_thread::sleep_for(500ms);
                    wg.Done();
                });
            });

            wg.Wait();
            fully_completed = true;
        });

        pool.WaitIdle();
    }
    ASSERT_TRUE(fully_completed);
}

TEST(TestDistributedPool, Racy) {
    bool fully_completed = false;
    {
        NExecutors::DistributedPool pool{4};

        pool.Start();

        NFibers::Go(pool, [&] {
            static const size_t kTasks = 100500;

            std::atomic<size_t> shared_counter{0};

            NFibers::WaitGroup wg;
            wg.Add(kTasks);

            for (size_t i = 0; i < kTasks; ++i) {
                NFibers::Go([&] {
                    const size_t old = shared_counter.load();
                    shared_counter.store(old + 1);

                    wg.Done();
                });
            }

            wg.Wait();

            std::cout << "Racy counter value: " << shared_counter << std::endl;

            ASSERT_LE(shared_counter.load(), kTasks);
            fully_completed = true;
        });

        pool.WaitIdle();
    }
    ASSERT_TRUE(fully_completed);
}

TEST(TestDistributedPool, ForBench) {
    bool fully_completed = false;
    {
        NExecutors::DistributedPool pool{4};

        static const size_t kTasks = 1'000;

        pool.Start();

        size_t counter{};

        NFibers::Go(pool, [&] {
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
            ASSERT_EQ(counter, kTasks);
            fully_completed = true;
        });

        pool.WaitIdle();
    }
    ASSERT_TRUE(fully_completed);
}
