//
// Created by konstantin on 08.06.24.
//

#pragma once

#include <fiber/awaiter/awaiter.h>

namespace fibers {

void Suspend(IAwaiter* waiter);

}