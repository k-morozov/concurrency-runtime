//
// Created by konstantin on 20.07.24.
//

#pragma once

#include <coroutine>
#include <exception>
#include <iostream>
#include <syncstream>
#include <thread>

namespace NComponents {

struct ResumableNoOwn {
    struct promise_type {
        std::suspend_never initial_suspend() const noexcept { return {}; }
        std::suspend_never final_suspend() const noexcept { return {}; }
        auto get_return_object() {
            return std::coroutine_handle<promise_type>::from_promise(*this);
        }
        void unhandled_exception() { std::terminate(); }
    };

    ResumableNoOwn(std::coroutine_handle<promise_type> /*handle*/) {
//        std::osyncstream(std::cout)
//            << "[ResumableNoOwn][this=" << this
//            << "][thread_id=" << std::this_thread::get_id() << "] create"
//            << std::endl;
    }
};

}  // namespace NComponents
