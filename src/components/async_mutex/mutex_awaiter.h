//
// Created by konstantin on 19.07.24.
//

#pragma once

#include <coroutine>

namespace NComponents {

struct Event;

struct MutexAwaiter final {
    Event& event;
    std::coroutine_handle<> coro{};

    explicit MutexAwaiter(Event& event);

    bool await_ready() const;
    void await_suspend(std::coroutine_handle<>);
    void await_resume();
};

}  // namespace NComponents