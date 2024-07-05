//
// Created by konstantin on 02.07.24.
//
#include "gtest/gtest.h"

#include <thread>
#include <unordered_map>

#include <components/lock_free/simple_ms_queue.h>

template<class T>
using Queue = NComponents::SimpleMSQueue<T>;

TEST(TestLockFreeQueue, Empty) {
    Queue<std::string> stack;

    auto empty = stack.TryPop();
    ASSERT_FALSE(empty);
}

TEST(TestLockFreeQueue, JustWorks) {
    Queue<std::string> stack;

    stack.Push("Data");
    auto item = stack.TryPop();
    ASSERT_TRUE(item);
    ASSERT_EQ(*item, "Data");

    auto empty = stack.TryPop();
    ASSERT_FALSE(empty);
}

TEST(TestLockFreeQueue, Dtor) {
    Queue<std::string> stack;

    stack.Push("One");
    stack.Push("Two");
}

TEST(TestLockFreeQueue, FIFO) {
    Queue<int> stack;

    stack.Push(1);
    stack.Push(2);
    stack.Push(3);

    ASSERT_EQ(*stack.TryPop(), 1);
    ASSERT_EQ(*stack.TryPop(), 2);
    ASSERT_EQ(*stack.TryPop(), 3);

    ASSERT_FALSE(stack.TryPop());
}

TEST(TestLockFreeQueue, TwoQueues) {
    Queue<int> queue_1;
    Queue<int> queue_2;

    queue_1.Push(3);
    queue_2.Push(11);
    ASSERT_EQ(*queue_1.TryPop(), 3);
    ASSERT_EQ(*queue_2.TryPop(), 11);
}

TEST(TestLockFreeQueue, ManyPush) {
    Queue<size_t> queue;

    std::unordered_map<size_t, bool> table;
    constexpr size_t kMaxNumber = 10'000;
    for(size_t i=0; i<kMaxNumber; i++) {
        table[i] = false;
    }

    ASSERT_EQ(table.size(), kMaxNumber);

    std::jthread th1([&]() {
        for(int i=0; i<1'000; i++) {
            queue.Push(i);
        }
    });
    std::jthread th2([&]() {
        for(size_t i=1'000; i<5'000; i++) {
            queue.Push(i);
        }
    });
    std::jthread th3([&]() {
        for(size_t i=5'000; i<kMaxNumber; i++) {
            queue.Push(i);
        }
    });


    size_t counter{};
    while(counter < kMaxNumber) {
        auto r = queue.TryPop();
        if (r) {
            ASSERT_FALSE(table[r.value()]);
            table[r.value()] = true;
            counter++;
        }
    }
    ASSERT_EQ(table.size(), kMaxNumber);
    for(auto [_, v] : table) {
        ASSERT_TRUE(v);
    }
}

TEST(TestLockFreeQueue, ManyPop) {
    Queue<size_t> queue;
    constexpr size_t kMaxNumber = 10'000;

    std::jthread th1([&]() {
        for(size_t i=0; i<kMaxNumber; i++) {
            queue.Push(i);
        }
    });

    std::vector<bool> table(kMaxNumber, false);
    std::atomic<size_t> counter{};
    std::mutex m;

    std::jthread th2([&]() {
        while(counter < kMaxNumber) {
            auto r = queue.TryPop();
            if (r) {
                {
                    std::lock_guard lock(m);
                    ASSERT_FALSE(table[r.value()]);
                    table[r.value()] = true;
                }
                counter++;
            }
        }
    });

    while(counter < kMaxNumber) {
        auto r = queue.TryPop();
        if (r) {
            {
                std::lock_guard lock(m);
                ASSERT_FALSE(table[r.value()]);
                table[r.value()] = true;
            }
            counter++;
        }
    }

    for(auto status_field : table) {
        ASSERT_TRUE(status_field);
    }
}