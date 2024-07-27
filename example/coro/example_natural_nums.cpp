//
// Created by konstantin on 13.07.24.
//

#include <coroutine>
#include <iostream>

#include "generator.h"

Generator<int> natural_nums() {
    int num = 0;
    for (;;) {
        co_yield num;
        num++;
    }
}

int main() {
    auto nums = natural_nums();
    for (size_t i{}; i < 5; i++) {
        nums.move_next();
        std::cout << nums.current_value() << std::endl;
    }
}