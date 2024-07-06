//
// Created by konstantin on 06.07.24.
//

#include "hazard.h"

#include <unordered_set>
#include <set>

namespace NComponents::detail {

struct ThreadState {
    std::atomic<void*>* ptr{};
};

std::mutex thread_lock;
std::unordered_set<ThreadState*> threads;

namespace detail {

thread_local std::atomic<void*> hazard_ptr{};

std::mutex scan_lock;

void ScanFreeList() {
    approximate_free_list_size.store(0);

    std::lock_guard lock(scan_lock);

    auto* retired = free_list.exchange(nullptr);

    std::set<void*> hazards;
    {
        std::lock_guard g(thread_lock);
        for (auto* state : threads) {
            if (void* hz = state->ptr->load(); hz) {
                hazards.insert(hz);
            }
        }
    }

    while (retired) {
        auto* next = retired->next;
        bool is_deleted = true;

        if (hazards.contains(retired->value)) {
            is_deleted = false;

            retired->next = free_list;
            while (!free_list.compare_exchange_weak(retired->next, retired)) {
            }

            approximate_free_list_size.fetch_add(1);
        }

        if (is_deleted) {
            retired->deleter();
            delete retired;
        }

        retired = next;
    }
}

}  // namespace detail

void RegisterThread() {
    std::lock_guard g(thread_lock);
    threads.insert(new ThreadState{.ptr = &detail::hazard_ptr});
}

void UnregisterThread() {
    std::unique_lock g(thread_lock);

    ThreadState* state_for_current_thread{nullptr};
    for (auto& state : threads) {
        if (state->ptr == &detail::hazard_ptr) {
            state_for_current_thread = state;
            threads.erase(state);
            break;
        }
    }

    delete state_for_current_thread;

    if (threads.empty()) {
        g.unlock();
        detail::ScanFreeList();
    }
}

}  // namespace NComponents::detail
