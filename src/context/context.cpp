//
// Created by konstantin on 14.05.24.
//

#include "context.h"

#include <cassert>
#include <stdexcept>

namespace NFibers::NContext {

void Context::Setup(BufferView target_buffer, Runner* runner) {
    assert(runner);
    runner_ = runner;
    buffer_ctx_.Setup(target_buffer, this);
    sanitizer_ctx_.Setup(target_buffer);
}

void Context::SwitchTo(Context& target) {
    sanitizer_ctx_.BeforeSwitch(target.sanitizer_ctx_);
    buffer_ctx_.SwitchTo(target.buffer_ctx_);
    sanitizer_ctx_.AfterSwitch();
}

void Context::ExitTo(Context& target) {
    sanitizer_ctx_.BeforeExit(target.sanitizer_ctx_);
    buffer_ctx_.SwitchTo(target.buffer_ctx_);
    std::abort();
}

void Context::Run() {
    assert(runner_);
    sanitizer_ctx_.AfterStart();
    runner_->Run();
}

}  // namespace NFibers::NContext