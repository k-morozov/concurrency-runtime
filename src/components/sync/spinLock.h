//
// Created by konstantin on 07.06.24.
//

#pragma once

#include <atomic>

namespace NSync {

class SpinLock {
public:
    void lock();

    void unlock();

    void try_lock();

    void Lock();

    void Unlock();

    bool TryLock();

private:
    std::atomic<bool> lock_{false};
};

}  // namespace NSync