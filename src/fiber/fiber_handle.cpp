//
// Created by konstantin on 06.06.24.
//

#include "fiber_handle.h"

#include <cassert>
#include <utility>

#include <fiber/awaiter_fiber.h>

namespace fibers {

AwaiterFiber* FiberHandle::Release() {
    assert(!IsInvalid());
    return std::exchange(fiber_, nullptr);
}
void FiberHandle::Schedule() {
    Release()->Schedule();
}
void FiberHandle::Switch() {
    Release()->Switch();
}

}  // namespace fibers
