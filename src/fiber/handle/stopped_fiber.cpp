//
// Created by konstantin on 06.06.24.
//

#include <cassert>
#include <utility>

#include <fiber/awaiter_fiber.h>
#include <fiber/handle/stopped_fiber.h>

namespace fibers {

AwaiterFiber* StoppedFiber::Release() {
    assert(!IsInvalid());
    return std::exchange(fiber_, nullptr);
}

void StoppedFiber::Schedule() {
    Release()->Schedule();
}

void StoppedFiber::Switch() {
    Release()->Switch();
}

}  // namespace fibers
