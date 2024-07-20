//
// Created by konstantin on 19.07.24.
//

#include "mutex_awaiter.h"

#include <iostream>

#include <components/async_mutex/event.h>

namespace NComponents {

MutexAwaiter::MutexAwaiter(Event& event) : event(event) {
    std::cout << "[MutexAwaiter] create" << std::endl;
}

bool MutexAwaiter::await_ready() const {
    const bool lock_own = event.TrySet();
    std::cout << "[MutexAwaiter::await_ready] try set event " << lock_own
              << ", so ready status=" << lock_own << std::endl;
    return lock_own;
}

void MutexAwaiter::await_suspend(std::coroutine_handle<> coro_) {
    std::cout << "[MutexAwaiter::await_suspend] call" << std::endl;
    coro = coro_;
    event.ParkAwaiter(*this);
}

void MutexAwaiter::await_resume() {
    std::cout << "[MutexAwaiter::await_resume] call" << std::endl;
    event.UnSet();
}

}  // namespace NComponents