//
// Created by konstantin on 09.07.24.
//

#include "hazard_manager.h"

#include <cassert>
#include <chrono>
#include <thread>
#include <unordered_set>

#include <components/lock_free/hazard/mutator.h>

using namespace std::chrono_literals;

namespace NComponents::NHazard {

HazardManager::HazardManager() {
    collector_thread.emplace(std::thread([this] { Collect(); }));
}

HazardManager::~HazardManager() {
    cancel_collect.store(true);

    if (collector_thread && collector_thread->joinable()) {
        collector_thread->join();
    }

    assert(mutators_count.load() == 0);

    std::lock_guard g(thread_lock);
    for (auto& [k, state] : threads) {
        assert(nullptr == state->protected_ptr.load());

        CheckToDelete(state, {});

        assert(nullptr == state->retired_ptrs.load());

        delete state;
    }
    threads.clear();
}

HazardManager* HazardManager::Get() {
    static HazardManager gc;
    return &gc;
}

Mutator HazardManager::MakeMutator() {
    {
        std::lock_guard g(thread_lock);
        if (!threads.contains(std::this_thread::get_id()))
            threads.insert({std::this_thread::get_id(), new ThreadState{}});
    }
    mutators_count.fetch_add(1);
    return Mutator(this);
}

void HazardManager::Collect() {
    while (!cancel_collect.load() && mutators_count.load() != 0) {
        approximate_free_list_size.store(0);

        {
            std::lock_guard lock(thread_lock);

            std::unordered_set<void*> protected_ptrs;
            {
                for (auto& [id, thread_state] : threads) {
                    if (void* hz = thread_state->protected_ptr.load(); hz) {
                        protected_ptrs.insert(hz);
                    }
                }
            }

            for (auto& [id, thread_state] : threads) {
                CheckToDelete(thread_state, protected_ptrs);
            }
        }

        while (approximate_free_list_size < LimitFreeList &&
               !cancel_collect.load()) {
            std::this_thread::sleep_for(500ms);
        }
    }
}
void HazardManager::CheckToDelete(
    ThreadState* thread_state,
    const std::unordered_set<void*>& protected_ptrs) {

    RetirePtr* candidate_retired = thread_state->retired_ptrs.exchange(nullptr);

    while (candidate_retired) {
        auto* next = candidate_retired->next;
        bool is_deleted = true;

        if (protected_ptrs.contains(candidate_retired->value)) {
            is_deleted = false;

            candidate_retired->next = thread_state->retired_ptrs;
            while (!thread_state->retired_ptrs.compare_exchange_weak(
                candidate_retired->next, candidate_retired)) {
            }

            approximate_free_list_size.fetch_add(1);
        }

        if (is_deleted) {
            candidate_retired->deleter();
            delete candidate_retired;
        }

        candidate_retired = next;
    }
}

}  // namespace NComponents::NHazard