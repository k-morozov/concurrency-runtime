//
// Created by konstantin on 06.07.24.
//

#pragma once

#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <shared_mutex>

#include <components/lock_free/hazard/retire.h>
#include <components/lock_free/hazard/scan.h>

namespace NComponents::NHazard {

inline constexpr size_t LimitFreeList = 8;
extern thread_local std::atomic<void*> hazard_ptr;

class Mutator final {
public:
    Mutator();
    ~Mutator();

    template <class T>
    T* Acquire(std::atomic<T*>* ptr) {
        auto* before_store_value = ptr->load();
        do {
            hazard_ptr.store(before_store_value);
            auto* after_store_value = ptr->load();

            // @TODO why ptr can changes?
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
        auto* retire =
            new RetirePtr{.value = value,
                          .deleter = [value, d = std::move(deleter)] { d(value); },
                          .next = free_list.load()};
        while (!free_list.compare_exchange_weak(retire->next, retire)) {
        }

        approximate_free_list_size.fetch_add(1);

        if (approximate_free_list_size > LimitFreeList) {
            ScanFreeList();
        }
    }

private:
    inline void Release() { hazard_ptr.store(nullptr); }
};


}  // namespace NComponents::NHazard