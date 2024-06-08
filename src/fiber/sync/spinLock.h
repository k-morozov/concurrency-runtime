//
// Created by konstantin on 07.06.24.
//

#pragma once

#include <atomic>

namespace fibers {

class SpinLock {
public:
    void lock() { Lock(); }

    void unlock() { Unlock(); }

    void try_lock() { TryLock(); }

    void Lock() {
        while (lock_.exchange(true)) {
            while (lock_.load()) {
            }
        }
    }

    void Unlock() { lock_.store(false); }

    bool TryLock() {
        bool old_value = false;
        return lock_.compare_exchange_strong(old_value, true);
    }

private:
    std::atomic<bool> lock_{false};
};

}  // namespace fibers