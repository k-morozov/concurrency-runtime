//
// Created by konstantin on 19.07.24.
//

#include "mutex_awaiter.h"

#include <cassert>
#include <iostream>
#include <syncstream>
#include <thread>

#include <components/async_mutex/async_mutex_coro_impl.h>
#include <components/async_mutex/config.h>

namespace NComponents {

MutexAwaiter::MutexAwaiter(AsyncMutexCoroImpl& event, LockGuard&& guard)
    : event(event), guard(std::move(guard)) {
    if (ENABLE_DEBUG) {
        std::osyncstream(std::cout)
            << *this << " create with guard." << std::endl;
    }
}

MutexAwaiter::MutexAwaiter(MutexAwaiter&& o) noexcept
    : event(o.event), coro(o.coro) {
    o.coro = nullptr;
    if (auto* p = o.guard.release(); p) {
        guard = MutexAwaiter::LockGuard(*p, std::adopt_lock);
    }
}

MutexAwaiter::~MutexAwaiter() {
    if (ENABLE_DEBUG) {
        std::osyncstream(std::cout) << *this << " destroy." << std::endl;
    }
}

void MutexAwaiter::ReleaseLock() const {
    if (ENABLE_DEBUG) {
        std::osyncstream(std::cout)
            << *this << "[await_suspend] release guard." << std::endl;
    }

    if (auto* p = guard.release(); p) {
        p->unlock();
    }
}

bool MutexAwaiter::await_ready() const {
    const bool lock_own = event.TryLock();

    if (ENABLE_DEBUG) {
        std::osyncstream(std::cout) << *this << "[await_ready] try lock mutex: "
                                    << (lock_own ? "OK" : "Fail") << std::endl;
    }

    if (lock_own) ReleaseLock();

    return lock_own;
}

void MutexAwaiter::await_suspend(std::coroutine_handle<> coro_) noexcept {
    if (ENABLE_DEBUG) {
        std::osyncstream(std::cout)
            << *this << "[await_suspend] park and release guard." << std::endl;
    }
    coro = coro_;
    event.ParkAwaiter(this);
}

void MutexAwaiter::await_resume() const noexcept {
    if (ENABLE_DEBUG) {
        std::osyncstream(std::cout)
            << *this << "[await_resume] call and just resume." << std::endl;
    }
}

std::ostream& operator<<(std::ostream& stream, const MutexAwaiter& /*w*/) {
    stream << "[MutexAwaiter][thread_id=" << std::this_thread::get_id() << "]";
    return stream;
}

}  // namespace NComponents