//
// Created by konstantin on 29.05.24.
//

#pragma once

#include <coro/stackfull_coro.h>
#include <executor/thread_pool/thread_pool.h>

namespace fibers {

void Go(pool::ThreadPool& scheduler, coro::Routine routine);

void Go(coro::Routine routine);

void Yield();

}  // namespace fibers
