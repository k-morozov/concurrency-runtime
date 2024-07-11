//
// Created by konstantin on 06.06.24.
//

#pragma once

#include <cassert>

namespace NFibers {

class AwaiterFiber;

class StoppedFiber {
    friend class AwaiterFiber;

public:
    StoppedFiber() : StoppedFiber(nullptr) {}

    static StoppedFiber Invalid() { return StoppedFiber(nullptr); }

    bool IsValid() const { return fiber_ != nullptr; }

    void Schedule();

    void Switch();

private:
    AwaiterFiber* fiber_{nullptr};

    explicit StoppedFiber(AwaiterFiber* fiber) : fiber_(fiber) {
        //        assert(fiber_);
    }

    AwaiterFiber* Release();
};

}  // namespace NFibers