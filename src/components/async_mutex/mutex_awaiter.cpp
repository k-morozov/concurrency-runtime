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
    std::osyncstream(std::cout) << *this << "create." << std::endl;
}

bool MutexAwaiter::await_ready() const {
    // td::lock_guard lock(spinlock); lock from here and to result
    const bool lock_own = event.TrySet();

    std::osyncstream(std::cout) << *this << "[await_ready] try lock mutex: " << (lock_own ? "OK" : "Fail") << std::endl;
    return lock_own;
}

void MutexAwaiter::await_suspend(std::coroutine_handle<> coro_) noexcept {
    std::osyncstream(std::cout) << *this << "[await_suspend] call and park." << std::endl;
    coro = coro_;
    event.ParkAwaiter(*this);
}

void MutexAwaiter::await_resume() const noexcept {
    std::osyncstream(std::cout) << *this << "[await_resume] call and just resume." << std::endl;
}

std::ostream& operator<<(std::ostream& stream, const MutexAwaiter& w) {
    stream << "[MutexAwaiter][thread_id="
           << std::this_thread::get_id() << "]";
    return stream;
}

}  // namespace NComponents