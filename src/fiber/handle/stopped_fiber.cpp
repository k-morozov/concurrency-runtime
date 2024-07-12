//
// Created by konstantin on 06.06.24.
//

#include <cassert>
#include <utility>

#include <fiber/awaiter_fiber.h>
#include <fiber/handle/stopped_fiber.h>

namespace NFibers {

AwaiterFiber* StoppedFiber::Release() {
    assert(IsValid());
    return std::exchange(fiber_, nullptr);
}

void StoppedFiber::Schedule() { Release()->Schedule(true); }

void StoppedFiber::Switch() { Release()->Switch(); }

}  // namespace NFibers
