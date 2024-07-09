//
// Created by konstantin on 06.07.24.
//

#include "mutator.h"

#include <components/lock_free/hazard/thread_state.h>
#include <components/lock_free/hazard/manager.h>

namespace NComponents::NHazard {

Mutator::Mutator(Manager* gc) : gc(gc) { RegisterThread(); }

Mutator::~Mutator() {
    Release();
    UnregisterThread();
}

void Mutator::RegisterThread() {
    std::lock_guard g(gc->thread_lock);
    gc->threads.insert(new ThreadState{.thread_hazard_ptr = &hazard_ptr});
}

void Mutator::UnregisterThread() {
    std::unique_lock g(gc->thread_lock);

    ThreadState* state_for_current_thread{nullptr};
    for (auto& state : gc->threads) {
        if (state->thread_hazard_ptr == &hazard_ptr) {
            state_for_current_thread = state;
            gc->threads.erase(state);
            break;
        }
    }

    delete state_for_current_thread;

    if (gc->threads.empty()) {
        g.unlock();
        ScanFreeList();
    }
}

}  // namespace NComponents::NHazard
