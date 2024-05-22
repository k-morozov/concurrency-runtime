//
// Created by konstantin on 14.05.24.
//

#include "context.h"

#include <cassert>

namespace go::impl::ctx {

void Context::Setup(BufferView target_buffer, Runner* runner) {
    assert(runner);
    buffer_ctx_.Setup(target_buffer, this);
    runner_ = runner;
}

void Context::SwitchTo(Context& target) {
    buffer_ctx_.SwitchTo(target.buffer_ctx_);
}

void Context::ExitTo(Context& /*target*/) {}

void Context::Run() { runner_->Run(); }

}  // namespace go::impl::ctx