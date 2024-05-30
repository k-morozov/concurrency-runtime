//
// Created by konstantin on 30.05.24.
//
#include "fiber_coro.h"

#include <cassert>

namespace go::impl::coro {

FiberCoroutine::FiberCoroutine(Routine routine, ctx::Buffer&& buffer)
    : routine_(std::move(routine)), coro_buffer_(std::move(buffer)) {
    coro_ctx_.Setup(coro_buffer_.GetSpan(), this);
}

void FiberCoroutine::Run() {
    try {
        routine_();
    } catch (...) {
        // current exception
    }

    is_completed_ = true;
    coro_ctx_.SwitchTo(caller_ctx_);
}

void FiberCoroutine::Resume() {
    caller_ctx_.SwitchTo(coro_ctx_);
    // exception?
}

void FiberCoroutine::Suspend() { coro_ctx_.SwitchTo(caller_ctx_); }

}  // namespace go::impl::coro