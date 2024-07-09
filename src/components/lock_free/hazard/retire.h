//
// Created by konstantin on 08.07.24.
//

#pragma once

#include <functional>

namespace NComponents::NHazard {

struct RetirePtr {
    void* value;
    std::function<void()> deleter;
    RetirePtr* next;
};

}  // namespace NComponents::NHazard