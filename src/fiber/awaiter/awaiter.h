//
// Created by konstantin on 06.06.24.
//

#pragma once

#include <fiber/handle/stopped_fiber.h>

namespace NFibers {

class IAwaiter {
public:
    virtual ~IAwaiter() = default;
    virtual void AwaitSuspend(StoppedFiber handle) = 0;
};

}  // namespace NFibers