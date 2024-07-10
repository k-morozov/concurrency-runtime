//
// Created by konstantin on 08.07.24.
//

#include "scan.h"

#include <set>

#include <components/lock_free/hazard/retire.h>
#include <components/lock_free/hazard/manager.h>
#include <components/lock_free/hazard/thread_state.h>

namespace NComponents::NHazard {

void ScanFreeList() {
    approximate_free_list_size.store(0);

    std::lock_guard lock(Manager::Get()->scan_lock);

    RetirePtr* retired = free_list.exchange(nullptr);

    // @todo manger logic
    std::set<void*> hazards;
    {
        std::lock_guard g(Manager::Get()->thread_lock);
        for (auto* thread_state : Manager::Get()->threads) {
            if (void* hz = thread_state->thread_hazard_ptr.load(); hz) {
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