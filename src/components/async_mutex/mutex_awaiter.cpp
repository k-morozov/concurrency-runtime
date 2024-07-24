//
// Created by konstantin on 19.07.24.
//

#include "mutex_awaiter.h"

#include <cassert>
#include <iostream>
#include <syncstream>
#include <thread>

#include <components/async_mutex/event.h>

namespace NComponents {

MutexAwaiter::MutexAwaiter(Event& event, LockGuard guard)
    : event(event), guard(std::move(guard)) {
    //    std::osyncstream(std::cout) << *this << " create." << std::endl;
}

MutexAwaiter::~MutexAwaiter() {
    //    std::osyncstream(std::cout) << *this << " destroy." << std::endl;
}

bool MutexAwaiter::await_ready() const {
    const bool lock_own = event.TryLock();

    std::osyncstream(std::cout) << *this << "[await_ready] try lock mutex: "
                                << (lock_own ? "OK" : "Fail") << std::endl;

    if (lock_own)
        guard.unlock();

    return lock_own;
}

void MutexAwaiter::await_suspend(std::coroutine_handle<> coro_) noexcept {
    std::osyncstream(std::cout)
        << *this << "[await_suspend] park and release guard." << std::endl;
    coro = coro_;
    event.ParkAwaiter(std::move(*this));
}

void MutexAwaiter::await_resume() const noexcept {
    std::osyncstream(std::cout)
        << *this
        << "[await_resume] call and just resume, status flag=" << event.IsSet()
        << ", release guard." << std::endl;
}

std::ostream& operator<<(std::ostream& stream, const MutexAwaiter& /*w*/) {
    stream << "[MutexAwaiter][thread_id=" << std::this_thread::get_id() << "]";
    return stream;
}

}  // namespace NComponents