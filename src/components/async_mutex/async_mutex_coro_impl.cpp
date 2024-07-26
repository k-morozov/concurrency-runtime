//
// Created by konstantin on 19.07.24.
//
#include "async_mutex_coro_impl.h"

#include <iostream>
#include <syncstream>

#include <components/async_mutex/config.h>

namespace NComponents {

bool AsyncMutexCoroImpl::TryLock() {
    if (lock_flag) {
        if (ENABLE_DEBUG) {
            std::osyncstream(std::cout)
                << "[AsyncMutexCoroImpl::TryLock][thread_id="
                << std::this_thread::get_id()
                << "] lock_flag was locked. Need park." << std::endl;
        }

        return false;
    }

    if (ENABLE_DEBUG) {
        std::osyncstream(std::cout)
            << "[AsyncMutexCoroImpl::TryLock][thread_id="
            << std::this_thread::get_id() << "] lock lock_flag." << std::endl;
    }
    lock_flag = true;
    return true;
}

void AsyncMutexCoroImpl::Unlock() {
    std::unique_lock lock(spinlock);

    if (ENABLE_DEBUG) {
        std::osyncstream(std::cout)
            << "[AsyncMutexCoroImpl::Unlock][thread_id="
            << std::this_thread::get_id() << "] call" << std::endl;
    }
    if (!waiters.empty()) {
        if (ENABLE_DEBUG) {
            std::cout << "[AsyncMutexCoroImpl::Unlock] Waiters size="
                      << waiters.size() << ", wake up first" << std::endl;
        }
        MutexAwaiter waiter = std::move(waiters.front());
        waiters.pop_front();
        lock.unlock();

        waiter.Resume();
        return;
    }

    if (ENABLE_DEBUG) {
        std::cout << "[AsyncMutexCoroImpl::Unlock] Waiters empty. Set "
                     "lock_flag to false"
                  << std::endl;
    }
    lock_flag = false;
}

void AsyncMutexCoroImpl::ParkAwaiter(MutexAwaiter* awaiter) {
    assert(awaiter);

    if (ENABLE_DEBUG) {
        std::osyncstream(std::cout)
            << "[AsyncMutexCoroImpl::ParkAwaiter][thead_id="
            << std::this_thread::get_id()
            << "] add waiter, new size=" << waiters.size() + 1 << std::endl;
    }

    waiters.emplace_back(std::move(*awaiter));
    waiters.back().ReleaseLock();
}

MutexAwaiter AsyncMutexCoroImpl::operator co_await() {
    MutexAwaiter::LockGuard guard(spinlock);
    return MutexAwaiter{*this, std::move(guard)};
}

}  // namespace NComponents