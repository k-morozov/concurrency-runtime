//
// Created by konstantin on 06.07.24.
//

#include "mutator.h"

#include <cassert>
#include <thread>

#include <components/lock_free/hazard/manager.h>
#include <components/lock_free/hazard/thread_state.h>

namespace NComponents::NHazard {

Mutator::Mutator(Manager* gc) : gc(gc) {
    RegisterThread();
}

Mutator::~Mutator() {
    Release();
    UnregisterThread();
//    assert(nullptr == mutator_thread_state.retired_ptrs.load());
}

void Mutator::RegisterThread() {
    std::lock_guard g(gc->thread_lock);
    mutator_thread_state = gc->threads[std::this_thread::get_id()];
//    gc->threads.insert(std::this_thread::get_id(), &mutator_thread_state);
}

void Mutator::UnregisterThread() {
    std::unique_lock g(gc->thread_lock);

    Release();
//    if (mutator_thread_state->retired_ptrs == nullptr) {
//        delete gc->threads[std::this_thread::get_id()];
//        gc->threads.erase(std::this_thread::get_id());
//    }

    if (!gc->threads.empty()) {
        g.unlock();
        gc->Collect();
    }
}

void Mutator::IncreaseRetired() {
    gc->approximate_free_list_size.fetch_add(1);

    if (gc->approximate_free_list_size > LimitFreeList) {
        gc->Collect();
    }
}

}  // namespace NComponents::NHazard
