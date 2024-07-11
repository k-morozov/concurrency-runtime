//
// Created by konstantin on 29.05.24.
//

#pragma once

#include <coro/stackfull_coro.h>
#include <executor/executor.h>

namespace NFibers {

void Go(NExecutors::IExecutor& scheduler, NCoro::Routine routine);

void Go(NCoro::Routine routine);

void Yield();

}  // namespace fibers
