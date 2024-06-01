//
// Created by konstantin on 29.05.24.
//

#pragma once

#include <coro/fiber_coro.h>
#include <pool/executor/executor.h>

namespace fibers {

void Go(pool::Executor& scheduler, coro::Routine routine);

void Go(coro::Routine routine);

void Yield();

}  // namespace fibers
