//
// Created by konstantin on 19.07.24.
//
#include <iostream>
#include <syncstream>

#include "async_mutex_coro_impl.h"

namespace NComponents {

bool AsyncMutexCoroImpl::TryLock() {
    if (lock_flag) {
        std::osyncstream(std::cout)
            << "[AsyncMutexCoroImpl::TryLock][thread_id=" << std::this_thread::get_id()
            << "] lock_flag was locked. Need park." << std::endl;
        return false;
    }

    std::osyncstream(std::cout)
        << "[AsyncMutexCoroImpl::TryLock][thread_id=" << std::this_thread::get_id()
        << "] lock lock_flag." << std::endl;
    lock_flag = true;
    return true;
}

void AsyncMutexCoroImpl::Unlock() {
    std::unique_lock lock(spinlock);

    std::osyncstream(std::cout)
        << "[AsyncMutexCoroImpl::Unlock][thread_id=" << std::this_thread::get_id()
        << "] call" << std::endl;

    if (!waiters.empty()) {
        std::cout << "[AsyncMutexCoroImpl::Unlock] Waiters size=" << waiters.size()
                  << ", wake up first" << std::endl;
        MutexAwaiter waiter = std::move(waiters.front());
        waiters.pop_front();
        lock.unlock();

        waiter.Resume();
        return;
    }

    std::cout << "[AsyncMutexCoroImpl::Unlock] Waiters empty. Set lock_flag to false"
              << std::endl;
    lock_flag = false;
}

void AsyncMutexCoroImpl::ParkAwaiter(MutexAwaiter* awaiter) {
    assert(awaiter);

    std::osyncstream(std::cout)
        << "[AsyncMutexCoroImpl::ParkAwaiter][thead_id=" << std::this_thread::get_id()
        << "] add waiter, new size=" << waiters.size() + 1 << std::endl;

    waiters.emplace_back(std::move(*awaiter));
    waiters.back().ReleaseLock();
}

MutexAwaiter AsyncMutexCoroImpl::operator co_await() {
    return MutexAwaiter{*this, spinlock};
}

}  // namespace NComponents