//
// Created by konstantin on 08.06.24.
//

#include "gtest/gtest.h"

#include <components/intrusive/list.h>

struct TestIntrusiveNode : intrusive::Node<TestIntrusiveNode> {
    explicit TestIntrusiveNode(size_t n)
        : intrusive::Node<TestIntrusiveNode>(), number(n) {}

    const size_t number;
};

TEST(TestList, PopEmpty) {
    intrusive::List<TestIntrusiveNode> list;
    ASSERT_EQ(list.IsEmpty(), true);
    auto* empty = list.Pop();
    ASSERT_EQ(list.IsEmpty(), true);
    ASSERT_EQ(empty, nullptr);
}

TEST(TestList, Push1Node) {
    TestIntrusiveNode node1(1);

    intrusive::List<TestIntrusiveNode> list;

    list.Push(&node1);

    ASSERT_EQ(list.IsEmpty(), false);

    auto* node1_ptr = list.Pop();

    ASSERT_EQ(list.IsEmpty(), true);

    ASSERT_EQ(node1_ptr->number, node1.number);
    ASSERT_EQ(node1_ptr, &node1);
}

TEST(TestList, PushPopNode1) {
    TestIntrusiveNode node1(1);
    TestIntrusiveNode node2(2);

    intrusive::List<TestIntrusiveNode> list;

    list.Push(&node1);
    list.Push(&node2);

    auto* node1_ptr = list.Pop();
    ASSERT_EQ(list.IsEmpty(), false);
    ASSERT_EQ(node1_ptr->number, node1.number);
    ASSERT_EQ(node1_ptr, &node1);

    auto* node2_ptr = list.Pop();
    ASSERT_EQ(list.IsEmpty(), true);
    ASSERT_EQ(node2_ptr->number, node2.number);
    ASSERT_EQ(node2_ptr, &node2);
}

TEST(TestList, PushPopNode2) {
    TestIntrusiveNode node1(117);
    TestIntrusiveNode node2(3726);

    intrusive::List<TestIntrusiveNode> list;

    list.Push(&node1);

    auto* node1_ptr = list.Pop();
    ASSERT_EQ(list.IsEmpty(), true);
    ASSERT_EQ(node1_ptr->number, node1.number);
    ASSERT_EQ(node1_ptr, &node1);

    list.Push(&node2);

    auto* node2_ptr = list.Pop();
    ASSERT_EQ(list.IsEmpty(), true);
    ASSERT_EQ(node2_ptr->number, node2.number);
    ASSERT_EQ(node2_ptr, &node2);
}

TEST(TestList, PushPopNodeMany) {
    std::vector<TestIntrusiveNode> expected;
    intrusive::List<TestIntrusiveNode> list;

    constexpr size_t max_count = 3;

    for(size_t i=0; i<max_count; i++) {
        TestIntrusiveNode node(i);
        expected.push_back(node);
    }

    for(size_t i=0; i<max_count; i++) {
        list.Push(&expected[i]);
    }

    for(size_t i=0; i<max_count; i++) {
        auto* node_ptr = list.Pop();
        ASSERT_EQ(node_ptr, &expected[i]);
        ASSERT_EQ(node_ptr->number, expected[i].number);
    }

    ASSERT_EQ(list.IsEmpty(), true);
}