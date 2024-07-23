//
// Created by konstantin on 19.07.24.
//

#pragma once

#include <coroutine>
#include <iostream>

namespace NComponents {

struct Event;

struct MutexAwaiter final {
    Event& event;
    std::coroutine_handle<> coro{};

    explicit MutexAwaiter(Event& event);
    ~MutexAwaiter();

    bool await_ready() const;
    void await_suspend(std::coroutine_handle<>) noexcept;
    void await_resume() const noexcept;
};

std::ostream& operator<<(std::ostream& stream, const MutexAwaiter& w);

}  // namespace NComponents