//
// Created by konstantin on 14.05.24.
//

#pragma once

#include <context/buffer/buffer.h>
#include <context/buffer/buffer_context.h>
#include <context/runner.h>
#include <context/sanitizer/sanitizer_context.h>

namespace fibers::ctx {

class Context final : private Runner {
public:
    Context() = default;

    void Setup(BufferView target_buffer, Runner* runner);

    /**
     * 1. Save current context in this.
     * 2. Activate target context.
     */
    void SwitchTo(Context& target);

    [[noreturn]] void ExitTo(Context& target);

private:
    void Run() override;

    SanitizerContext sanitizer_ctx_{};
    BufferContext buffer_ctx_{};

    Runner* runner_{nullptr};
};

}  // namespace fibers::ctx