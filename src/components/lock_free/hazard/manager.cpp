//
// Created by konstantin on 09.07.24.
//

#include "manager.h"

#include <thread>
#include <unordered_set>

#include <components/lock_free/hazard/mutator.h>

namespace NComponents::NHazard {

Manager* Manager::Get() {
    static Manager gc;
    return &gc;
}

Mutator Manager::MakeMutator() {
    {
        std::lock_guard g(thread_lock);
        if (!threads.contains(std::this_thread::get_id()))
            threads.insert({std::this_thread::get_id(), new ThreadState{}});
    }
    return Mutator(this);
}

void Manager::Collect() {
    approximate_free_list_size.store(0);

    std::scoped_lock lock(scan_lock, thread_lock);

    // @todo manger logic
    std::unordered_set<void*> protected_ptrs;
    {
        for (auto& [id, thread_state] : threads) {
            if (void* hz = thread_state->protected_ptr.load(); hz) {
                protected_ptrs.insert(hz);
            }
        }
    }

    for (auto& [id, thread_state] : threads) {
        RetirePtr* candidate_retired =
            thread_state->retired_ptrs.exchange(nullptr);

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
}
Manager::~Manager() {
    for (auto& [k, state] : threads) {
        delete state;
    }
}

}  // namespace NComponents::NHazard