//
// Created by konstantin on 06.07.24.
//

#pragma once

#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <shared_mutex>

namespace NComponents {

namespace detail {

inline constexpr size_t LimitFreeList = 32;
extern thread_local std::atomic<void*> hazard_ptr;

struct RetirePtr {
    void* value;
    std::function<void()> deleter;
    RetirePtr* next;
};

inline std::atomic<RetirePtr*> free_list = nullptr;
inline std::atomic<size_t> approximate_free_list_size{0};

void ScanFreeList();

}  // namespace detail

void RegisterThread();
void UnregisterThread();

template <class T>
T* Acquire(std::atomic<T*>* ptr) {
    auto* before_store_value = ptr->load();
    do {
        detail::hazard_ptr.store(before_store_value);
        auto* after_store_value = ptr->load();

        // @TODO why ptr can changes?
        if (after_store_value == before_store_value) {
            return after_store_value;
        }

        before_store_value = after_store_value;
    } while (true);
}

inline void Release() { detail::hazard_ptr.store(nullptr); }

template <class T, class Deleter = std::default_delete<T>>
void Retire(T* value, Deleter deleter = {}) {
    if (!value) {
        return;
    }
    auto* retire = new detail::RetirePtr{
        .value = value,
        .deleter = [value, d = std::move(deleter)] { d(value); },
        .next = detail::free_list.load()};
    while (!detail::free_list.compare_exchange_weak(retire->next, retire)) {
    }

    detail::approximate_free_list_size.fetch_add(1);

    if (detail::approximate_free_list_size > detail::LimitFreeList) {
        detail::ScanFreeList();
    }
}

}  // namespace NComponents