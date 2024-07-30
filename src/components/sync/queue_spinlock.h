//
// Created by konstantin on 28.07.24.
//

#pragma once

#include <atomic>

namespace NSync {

class QueueSpinLock final {
public:
    class Guard final {
    public:
        explicit Guard(QueueSpinLock& host) : host(host) { host.Acquire(this); }
        ~Guard() {
            if (is_owner.load()) Release();
        }

        void Release() {
            host.Release(this);
            is_owner.store(false, std::memory_order_release);
        }

        void SetOwner() { is_owner.store(true, std::memory_order_release); }

        void SetNext(Guard* guard) { next.store(guard); }

        bool IsOwner() const {
            return is_owner.load(std::memory_order_acquire);
        }

        bool HasNext() const { return next.load() != nullptr; }

        void SetNextOwner() { next.load()->SetOwner(); }

    private:
        QueueSpinLock& host;
        std::atomic<Guard*> next{};
        std::atomic<bool> is_owner{};
    };

private:
    void Acquire(Guard* guard) {
        auto ancestor = tail_.exchange(guard, std::memory_order_acquire);
        if (ancestor == nullptr) {
            guard->SetOwner();
            return;
        }

        ancestor->SetNext(guard);
        while (!guard->IsOwner()) {
        }
    }

    void Release(Guard* guard) {
        if (guard->HasNext()) {
            guard->SetNextOwner();
            return;
        }

        Guard* old_guard = guard;
        while (!tail_.compare_exchange_weak(old_guard, nullptr,
                                            std::memory_order_release)) {
            if (guard->HasNext()) {
                guard->SetNextOwner();
                return;
            }
            old_guard = guard;
        }
    }

    std::atomic<Guard*> tail_{};
};

}  // namespace NSync
