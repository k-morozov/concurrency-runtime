//
// Created by konstantin on 17.06.24.
//

#include "spinLock.h"

namespace NSync {

void SpinLock::lock() { Lock(); }

void SpinLock::unlock() { Unlock(); }

void SpinLock::try_lock() { TryLock(); }

void SpinLock::Lock() {
    while (lock_.exchange(true, std::memory_order_acquire)) {
        while (lock_.load(std::memory_order_relaxed)) {
        }
    }
}

void SpinLock::Unlock() { lock_.store(false, std::memory_order_release); }

bool SpinLock::TryLock() {
    bool old_value = false;
    return lock_.compare_exchange_strong(old_value, true);
}

}  // namespace NSync