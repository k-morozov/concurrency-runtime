//
// Created by konstantin on 14.05.24.
//

#include "context.h"

#include <cassert>
#include <stdexcept>

namespace fibers::ctx {

void Context::Setup(BufferView target_buffer, Runner* runner) {
    assert(runner);
    runner_ = runner;
    buffer_ctx_.Setup(target_buffer, this);
}

void Context::SwitchTo(Context& target) {
    buffer_ctx_.SwitchTo(target.buffer_ctx_);
}

void Context::ExitTo(Context& target) {
    buffer_ctx_.SwitchTo(target.buffer_ctx_);
    std::abort();
}

void Context::Run() {
    assert(runner_);
    runner_->Run();
}

}  // namespace fibers::ctx