//
// Created by konstantin on 23.06.24.
//

#include "gtest/gtest.h"

#include <list>

#include <components/intrusive/list.h>

namespace {

struct TestNode : public NComponents::Node<TestNode>{
    size_t value{};
};
}

TEST(TestIntrusiveList, Create) {
    NComponents::IntrusiveList<TestNode> list;

    ASSERT_EQ(0u, list.Size());
    ASSERT_TRUE(list.IsEmpty());

    ASSERT_EQ(nullptr, list.Pop());

    ASSERT_EQ(0u, list.Size());
    ASSERT_TRUE(list.IsEmpty());
}

TEST(TestIntrusiveList, PushPop) {
    NComponents::IntrusiveList<TestNode> list;

    TestNode node{.value=11};

    list.Push(&node);

    ASSERT_EQ(1u, list.Size());
    ASSERT_FALSE(list.IsEmpty());

    node.value = 12u;

    auto p = list.Pop();

    ASSERT_EQ(12u, p->value);
    ASSERT_EQ(&node, p);

    ASSERT_EQ(0u, list.Size());
    ASSERT_TRUE(list.IsEmpty());
}

TEST(TestIntrusiveList, ManyPushPop) {
    NComponents::IntrusiveList<TestNode> list;
    std::list<TestNode> nodes;
    for(size_t i{}; i<10'000; i++) {
        nodes.push_back(TestNode{.value=i});
        list.Push(&nodes.back());
        ASSERT_EQ(i+1, list.Size());
    }
    size_t expected{};
    while(!list.IsEmpty()) {
        auto* result = list.Pop();
        ASSERT_EQ(result->value, nodes.front().value);
        ASSERT_EQ(result->value, expected);
        ASSERT_EQ(&nodes.front(), result);
        nodes.pop_front();
        ASSERT_EQ(nodes.size(), list.Size());
        expected++;
    }
    ASSERT_EQ(10'000u, expected);
}