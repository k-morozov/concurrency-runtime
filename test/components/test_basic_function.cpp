//
// Created by konstantin on 19.06.24.
//
#include "gtest/gtest.h"

#include <components/function/basic_function.h>

namespace {

int plus2(int a) { return a + 2; }

struct PlusThree {
    int operator()(int a) {
       return a + 3;
    }
};

}  // namespace

TEST(TestBasicFunction, Function) {
    NComponents::BasicFunction<int(int)> plusTwo(plus2);
    const auto result = plusTwo(3);
    ASSERT_EQ(result, 5);
}

TEST(TestBasicFunction, Lambda) {
    NComponents::BasicFunction<int(int)> plusFive([](int a) {
        return a + 5;
    });
    const auto result = plusFive(3);
    ASSERT_EQ(result, 8);
}

TEST(TestBasicFunction, Functor) {
    NComponents::BasicFunction<int(int)> plusThree(PlusThree{});
    const auto result = plusThree(3);
    ASSERT_EQ(result, 6);
}
