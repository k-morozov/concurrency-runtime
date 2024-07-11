//
// Created by konstantin on 19.06.24.
//
#include "gtest/gtest.h"

#include <components/function/basic_function.h>

namespace {

int plus2(int a) { return a + 2; }

struct PlusThree {
    int operator()(int a) { return a + 3; }
};

struct PlusThreeWithResource {
    PlusThreeWithResource() { resource = new int(1117); }
    ~PlusThreeWithResource() { delete resource; }

    PlusThreeWithResource(const PlusThreeWithResource& o) {
        if (o.resource) {
            resource = new int;
            *resource = *o.resource;
        }
    }

    PlusThreeWithResource(PlusThreeWithResource&& o) noexcept {
        if (o.resource) {
            resource = o.resource;
            o.resource = nullptr;
        }
    }

    int operator()(int a) { return a + 3; }

    int* resource{};
};

}  // namespace

TEST(TestBasicFunction, Function) {
    NComponents::BasicFunction<int(int)> plusTwo(plus2);
    const auto result = plusTwo(3);
    ASSERT_EQ(result, 5);
}

TEST(TestBasicFunction, Lambda) {
    NComponents::BasicFunction<int(int)> plusFive([](int a) { return a + 5; });
    const auto result = plusFive(3);
    ASSERT_EQ(result, 8);
}

TEST(TestBasicFunction, Functor) {
    NComponents::BasicFunction<int(int)> plusThree(PlusThree{});
    const auto result = plusThree(3);
    ASSERT_EQ(result, 6);
}

TEST(TestBasicFunction, FunctorResource) {
    NComponents::BasicFunction<int(int)> plusThreeWithResource(
        PlusThreeWithResource{});
    const auto result = plusThreeWithResource(3);
    ASSERT_EQ(result, 6);
}