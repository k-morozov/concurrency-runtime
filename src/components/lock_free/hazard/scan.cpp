//
// Created by konstantin on 08.07.24.
//

#include "scan.h"

#include <mutex>
#include <set>

#include <components/lock_free/hazard/retire.h>
#include <components/lock_free/hazard/thread_state.h>

namespace NComponents::NHazard {

namespace {
std::mutex scan_lock;
}

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

}  // namespace NComponents::NHazard