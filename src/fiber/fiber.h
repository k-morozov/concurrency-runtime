//
// Created by konstantin on 29.05.24.
//

#pragma once

#include <context/buffer/buffer.h>
#include <coro/stackfull_coro.h>
#include <executor/pool/thread_pool.h>

namespace fibers {

class Fiber {
public:
    Fiber(pool::ThreadPool* executor, coro::Routine routine,
          ctx::Buffer&& buffer);

    void Schedule();

    void Run();

    static Fiber* Self();

    static void Yield();

    pool::ThreadPool* GetScheduler();

private:
    pool::ThreadPool* executor_;
    coro::StackfullCoroutine fiber_coro_;
};

}  // namespace fibers