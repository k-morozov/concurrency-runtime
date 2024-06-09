//
// Created by konstantin on 08.06.24.
//

#pragma once

#include <fiber/awaiter/awaiter.h>

namespace fibers {

class YieldAwaiter : public IAwaiter {
public:
    void AwaitSuspend(StoppedFiber handle) override { handle.Schedule(); }
};

}  // namespace fibers
