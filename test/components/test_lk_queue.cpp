//
// Created by konstantin on 02.07.24.
//
#include "gtest/gtest.h"

#include <thread>

#include <components/lock_free/queue.h>

TEST(TestLockFreeQueue, Empty) {
    NComponents::LockFreeQueue<std::string> stack;

    auto empty = stack.TryPop();
    ASSERT_FALSE(empty);
}

TEST(TestLockFreeQueue, JustWorks) {
    NComponents::LockFreeQueue<std::string> stack;

    stack.Push("Data");
    auto item = stack.TryPop();
    ASSERT_TRUE(item);
    ASSERT_EQ(*item, "Data");

    auto empty = stack.TryPop();
    ASSERT_FALSE(empty);
}

TEST(TestLockFreeQueue, Dtor) {
    NComponents::LockFreeQueue<std::string> stack;

    stack.Push("One");
    stack.Push("Two");
}