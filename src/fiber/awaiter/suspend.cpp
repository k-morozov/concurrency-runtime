//
// Created by konstantin on 08.06.24.
//

#include "suspend.h"

#include <cassert>

#include "awaiter_fiber.h"

namespace NFibers {

void Suspend(IAwaiter* waiter) {
    auto current_fiber = AwaiterFiber::Self();
    assert(current_fiber);
    current_fiber->Suspend(waiter);
}

}  // namespace NFibers