//
// Created by konstantin on 28.07.24.
//

#include "gtest/gtest.h"

#include <thread>

#include <components/sync/queue_spinlock.h>

TEST(TestQueueSpinlock, LockUnlock) {
    NSync::QueueSpinLock spinlock;

    {
        NSync::QueueSpinLock::Guard guard(spinlock);    // <-- Acquired
                                                        // Critical section
    }  // <-- Released
}

TEST(TestQueueSpinlock, SequentialLockUnlock) {
    NSync::QueueSpinLock spinlock;

    {
        NSync::QueueSpinLock::Guard guard(spinlock);
        // Critical section
    }

    {
        NSync::QueueSpinLock::Guard guard(spinlock);
        // Critical section
    }
}

TEST(TestQueueSpinlock, ConcurrentIncrements) {
    NSync::QueueSpinLock spinlock;
    size_t counter = 0;

    const size_t kIncrementsPerThread = 1000;

    auto contender = [&] {
        for (size_t i = 0; i < kIncrementsPerThread; ++i) {
            NSync::QueueSpinLock::Guard guard(spinlock);

            size_t current = counter;
            std::this_thread::yield();
            counter = current + 1;
        }
    };

    std::thread t1(contender);
    std::thread t2(contender);
    t1.join();
    t2.join();

    std::cout << "Shared counter value: " << counter << std::endl;

    ASSERT_EQ(counter, 2 * kIncrementsPerThread);
}