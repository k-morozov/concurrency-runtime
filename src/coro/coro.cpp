//
// Created by konstantin on 28.05.24.
//

#include "coro.h"

#include <cassert>

namespace fibers::coro {

static const size_t kDefaultCoroBufferSize = 64 * 1024;

thread_local Coroutine* current_coro;

Coroutine::Coroutine(Coroutine::Routine routine)
    : routine_(std::move(routine)),
      coro_buffer_(ctx::Buffer::AllocBytes(kDefaultCoroBufferSize)) {
    coro_ctx_.Setup(coro_buffer_.GetSpan(), this);
}

void Coroutine::Run() {
    try {
        routine_();
    } catch (...) {
        // current exception
    }

    is_completed_ = true;
    coro_ctx_.ExitTo(caller_ctx_);
}

void Coroutine::Resume() {
    Coroutine* caller_coro = current_coro;
    current_coro = this;

    caller_ctx_.SwitchTo(coro_ctx_);

    current_coro = caller_coro;

    // exception?
}

void Coroutine::Suspend() {
    assert(current_coro);
    current_coro->coro_ctx_.SwitchTo(current_coro->caller_ctx_);
}

}  // namespace fibers::coro
