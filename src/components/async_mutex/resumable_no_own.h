//
// Created by konstantin on 20.07.24.
//

#pragma once

#include <coroutine>
#include <exception>

namespace NComponents {

struct ResumableNoOwn {
    struct promise_type {
        std::suspend_never initial_suspend() const noexcept { return {}; }
        std::suspend_always final_suspend() const noexcept { return {}; }
        auto get_return_object() { return std::coroutine_handle<promise_type>::from_promise(*this); }
        void unhandled_exception() { std::terminate(); }
    };

    ResumableNoOwn(std::coroutine_handle<promise_type> /*handle*/) {}
};

}  // namespace NComponents
