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
        explicit Guard(QueueSpinLock& host) : host(host) {
            host.Acquire(this);
        }
        ~Guard() {
            Release();
        }

        void Release() {
            host.Release(this);
        }

        void SetOwner() {
            is_owner.store(true);
        }

        void SetNext(Guard* guard) {
            next.store(guard);
        }

        bool IsOwner() const {
            return is_owner.load();
        }

        bool HasNext() const {
            return next.load() != nullptr;
        }

        void SetNextOwner() {
            next.load()->SetOwner();
        }

    private:
        QueueSpinLock& host;
        std::atomic<Guard*> next{};
        std::atomic<bool> is_owner{};
    };
private:
    void Acquire(Guard* guard) {
        auto ancestor = tail_.exchange(guard);
        if (ancestor == nullptr) {
            guard->SetOwner();
            return;
        }

        ancestor->SetNext(guard);
        while(!guard->IsOwner()) {}
    }

    void Release(Guard* guard) {
        if (guard->HasNext()) {
            guard->SetNextOwner();
            return;
        }

        Guard* old_guard = guard;
        while(!tail_.compare_exchange_weak(old_guard, nullptr)) {
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
