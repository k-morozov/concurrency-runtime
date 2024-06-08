//
// Created by konstantin on 06.06.24.
//

#pragma once

namespace fibers {

class AwaiterFiber;

class FiberHandle {
    friend class AwaiterFiber;

public:
    FiberHandle() : FiberHandle(nullptr) {}

    static FiberHandle Invalid() { return FiberHandle(nullptr); }

    bool IsInvalid() const { return fiber_ == nullptr; }

    void Schedule();

    void Switch();

private:
    AwaiterFiber* fiber_;

    explicit FiberHandle(AwaiterFiber* fiber) : fiber_(fiber) {}

    AwaiterFiber* Release();
};

}  // namespace fibers