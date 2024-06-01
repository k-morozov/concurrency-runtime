//
// Created by konstantin on 30.05.24.
//
#include <cassert>

#include "stackfull_coro.h"

namespace fibers::coro {

StackfullCoroutine::StackfullCoroutine(Routine routine, ctx::Buffer&& buffer)
    : routine_(std::move(routine)), coro_buffer_(std::move(buffer)) {
    coro_ctx_.Setup(coro_buffer_.GetSpan(), this);
}

void StackfullCoroutine::Run() {
    try {
        routine_();
    } catch (...) {
        // current exception
    }

    is_completed_ = true;
    coro_ctx_.SwitchTo(caller_ctx_);
}

void StackfullCoroutine::Resume() {
    caller_ctx_.SwitchTo(coro_ctx_);
    // exception?
}

void StackfullCoroutine::Suspend() { coro_ctx_.SwitchTo(caller_ctx_); }

}  // namespace fibers::coro