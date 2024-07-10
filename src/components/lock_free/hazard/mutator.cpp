//
// Created by konstantin on 06.07.24.
//

#include "mutator.h"

#include <components/lock_free/hazard/manager.h>
#include <components/lock_free/hazard/thread_state.h>

namespace NComponents::NHazard {

Mutator::Mutator(Manager* gc) : gc(gc) {
    RegisterThread();
}

Mutator::~Mutator() {
    Release();
    UnregisterThread();
}

void Mutator::RegisterThread() {
    std::lock_guard g(gc->thread_lock);
    gc->threads.insert(&mutator_thread_state);
}

void Mutator::UnregisterThread() {
    std::unique_lock g(gc->thread_lock);

    for (auto& state : gc->threads) {
        if (state->protected_ptr ==
            &mutator_thread_state.thread_hazard_ptr) {
            gc->threads.erase(state);
            break;
        }
    }

    mutator_thread_state.thread_hazard_ptr = nullptr;

    if (gc->threads.empty()) {
        g.unlock();
        ScanFreeList();
    }
}

}  // namespace NComponents::NHazard
