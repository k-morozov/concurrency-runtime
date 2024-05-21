//
// Created by konstantin on 14.05.24.
//

#include "context.h"

#include <cassert>

namespace go::impl::ctx {

void Context::Setup(BufferView target_buffer, Runner* runner) {
    assert(runner);
    engine_ctx_.Setup(target_buffer, runner);
    runner_ = runner;
}

void Context::SwitchTo(Context& /*target*/) {}

void Context::ExitTo(Context& /*target*/) {}
void Context::Run() {}

}  // namespace go::impl::ctx