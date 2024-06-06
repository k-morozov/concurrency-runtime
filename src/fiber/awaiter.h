//
// Created by konstantin on 06.06.24.
//

#pragma once

#include <fiber/fiber_handle.h>

namespace fibers {

class IAwaiter {
public:
    virtual ~IAwaiter() = default;
    virtual void AwaitSuspend(FiberHandle handle) = 0;
};

}  // namespace fibers