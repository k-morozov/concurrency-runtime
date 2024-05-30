//
// Created by konstantin on 30.05.24.
//

//
// Created by konstantin on 28.05.24.
//

#pragma once

#include <functional>

#include <context/buffer/buffer.h>
#include <context/context.h>
#include <context/runner.h>

namespace go::impl::coro {

class FiberCoroutine : private Runner {
public:
    using Routine = std::function<void()>;

    FiberCoroutine(Routine routine, ctx::Buffer&& buffer);

    void Resume();
    void Suspend();

    bool IsCompleted() const { return is_completed_; }

private:
    Routine routine_;
    ctx::Buffer coro_buffer_;
    ctx::Context coro_ctx_;
    ctx::Context caller_ctx_;

    bool is_completed_{false};

    void Run() override;
};

}  // namespace go::impl::coro
