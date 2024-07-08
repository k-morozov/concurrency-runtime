//
// Created by konstantin on 08.07.24.
//

#include "thread_state.h"

#include <components/lock_free/hazard/scan.h>

namespace NComponents::NHazard {

void RegisterThread() {
    std::lock_guard g(thread_lock);
    threads.insert(new ThreadState{.ptr = &hazard_ptr});
}

void UnregisterThread() {
    std::unique_lock g(thread_lock);

    ThreadState* state_for_current_thread{nullptr};
    for (auto& state : threads) {
        if (state->ptr == &hazard_ptr) {
            state_for_current_thread = state;
            threads.erase(state);
            break;
        }
    }

    delete state_for_current_thread;

    if (threads.empty()) {
        g.unlock();
        ScanFreeList();
    }
}

}  // namespace NComponents::NHazard