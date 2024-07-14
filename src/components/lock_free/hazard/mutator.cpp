//
// Created by konstantin on 06.07.24.
//

#include "mutator.h"

#include <cassert>
#include <iostream>
#include <thread>

#include <components/lock_free/hazard/hazard_manager.h>
#include <components/lock_free/hazard/thread_state.h>

namespace NComponents::NHazard {

Mutator::Mutator(HazardManager* gc) : gc(gc) {
    RegisterThread();
}

Mutator::~Mutator() {
    Release();
    UnregisterThread();
    const auto prev = gc->mutators_count.fetch_sub(1);
    std::cout << "prev=" << prev << std::endl;
    assert(prev > 0);
}

void Mutator::RegisterThread() {
    std::lock_guard g(gc->thread_lock);
    mutator_thread_state = gc->threads[std::this_thread::get_id()];
}

void Mutator::UnregisterThread() {
    // I suppose that we can stay thread state in threads here.
}

void Mutator::IncreaseRetired() {
    gc->approximate_free_list_size.fetch_add(1);
}

}  // namespace NComponents::NHazard
