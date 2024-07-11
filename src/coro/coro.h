//
// Created by konstantin on 28.05.24.
//

#pragma once

#include <functional>

#include <context/buffer/buffer.h>
#include <context/context.h>
#include <context/runner.h>
#include <coro/routine.h>

namespace NFibers::NCoro {

class Coroutine : private NContext::Runner {
public:
    using Routine = std::function<void()>;

    explicit Coroutine(Routine routine);

    void Resume();
    static void Suspend();

    bool IsCompleted() const { return is_completed_; }

private:
    Routine routine_;
    NContext::Buffer coro_buffer_;
    NContext::Context coro_ctx_;
    NContext::Context caller_ctx_;

    bool is_completed_{false};

    void Run() override;
};

}  // namespace NFibers::NCoro
