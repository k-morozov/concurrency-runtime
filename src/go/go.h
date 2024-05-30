//
// Created by konstantin on 29.05.24.
//

#pragma once

#include "coro/coro.h"
#include "pool/executor/executor.h"

void Go(pool::Executor& scheduler, go::impl::coro::Coroutine::Routine routine);

void Go(go::impl::coro::Coroutine::Routine routine);