//
// Created by konstantin on 06.07.24.
//

#pragma once

#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <shared_mutex>

#include <components/lock_free/hazard/fwd.h>
#include <components/lock_free/hazard/retire.h>
#include <components/lock_free/hazard/thread_state.h>

namespace NComponents::NHazard {

inline std::mutex log;

class Mutator final {
    HazardManager* gc;

protected:
    ThreadState* mutator_thread_state{};

public:
    explicit Mutator(HazardManager* gc);
    Mutator(const Mutator& o) = delete;
    Mutator(Mutator&& o) noexcept = delete;

    Mutator& operator=(const Mutator& o) = delete;
    Mutator& operator=(Mutator&& o) noexcept = delete;

    ~Mutator();

    template <class T>
    T* Acquire(std::atomic<T*>* ptr) {
        auto* before_store_value = ptr->load();
        do {
            mutator_thread_state->protected_ptr.store(before_store_value);
            auto* after_store_value = ptr->load();

            if (after_store_value == before_store_value) {
                return after_store_value;
            }

            before_store_value = after_store_value;
        } while (true);
    }

    template <class T, class Deleter = std::default_delete<T>>
    void Retire(T* value, Deleter deleter = {}) {
        if (!value) {
            return;
        }
        auto* retire = new RetirePtr{
            .value = value,
            .deleter = [value, d = std::move(deleter)] { d(value); },
            .next = mutator_thread_state->retired_ptrs.load()};
        while (!mutator_thread_state->retired_ptrs.compare_exchange_weak(
            retire->next, retire)) {
        }

        IncreaseRetired();
    }

    inline void Release() {
        mutator_thread_state->protected_ptr.store(nullptr);
    }



private:
    void RegisterThread();
    void UnregisterThread();

    void IncreaseRetired();
};

}  // namespace NComponents::NHazard