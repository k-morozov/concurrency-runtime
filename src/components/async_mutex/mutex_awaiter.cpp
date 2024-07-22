//
// Created by konstantin on 19.07.24.
//

#include "mutex_awaiter.h"

#include <iostream>
#include <syncstream>
#include <thread>

#include <components/async_mutex/event.h>

namespace NComponents {

MutexAwaiter::MutexAwaiter(Event& event) : event(event) {
    std::osyncstream(std::cout)
        << "[MutexAwaiter][this=" << this
        << "][thread=" << std::this_thread::get_id() << "] create" << std::endl;
}

bool MutexAwaiter::await_ready() const {
    // td::lock_guard lock(spinlock); lock from here and to result
    const bool lock_own = event.TrySet();
    std::osyncstream(std::cout)
        << "[MutexAwaiter::await_ready][this=" << this
        << "][thread=" << std::this_thread::get_id()
        << "] try lock mutex: " << (lock_own ? "OK" : "Fail") << std::endl;
    return lock_own;
}

void MutexAwaiter::await_suspend(std::coroutine_handle<> coro_) noexcept {
    std::osyncstream(std::cout) << "[MutexAwaiter::await_suspend][this=" << this
                                << "][thread=" << std::this_thread::get_id()
                                << "] call and park." << std::endl;
    coro = coro_;
    event.ParkAwaiter(*this);
}

void MutexAwaiter::await_resume() const noexcept {
    std::osyncstream(std::cout) << "[MutexAwaiter::await_resume][this=" << this
                                << "][thread=" << std::this_thread::get_id()
                                << "] call and just resume." << std::endl;
}

}  // namespace NComponents