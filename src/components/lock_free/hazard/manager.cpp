//
// Created by konstantin on 09.07.24.
//

#include "manager.h"

#include <set>

#include <components/lock_free/hazard/mutator.h>

namespace NComponents::NHazard {

Manager* Manager::Get() {
    static Manager gc;
    return &gc;
}

Mutator Manager::MakeMutator() {
    return Mutator(this);
}

void Manager::ScanFreeList() {
    approximate_free_list_size.store(0);

    std::scoped_lock lock(scan_lock, thread_lock);

    // @todo manger logic
    std::set<void*> hazards;
    {
        for (auto* thread_state : threads) {
            if (void* hz = thread_state->thread_hazard_ptr.load(); hz) {
                hazards.insert(hz);
            }
        }
    }

    for (auto* state : threads) {
        RetirePtr* candidate_retired = state->retired_ptrs.exchange(nullptr);
        while (candidate_retired) {
            auto* next = candidate_retired->next;
            bool is_deleted = true;

            if (hazards.contains(candidate_retired->value)) {
                is_deleted = false;

                candidate_retired->next = state->retired_ptrs;
                while (!state->retired_ptrs.compare_exchange_weak(
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

}  // namespace NComponents::NHazard