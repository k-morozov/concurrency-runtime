//
// Created by konstantin on 08.07.24.
//

#pragma once

#include <atomic>

namespace NComponents::NHazard {

struct RetirePtr;

struct ThreadState {
    std::atomic<void*> thread_hazard_ptr{};
    std::atomic<void*>* protected_ptr{&thread_hazard_ptr};
    std::atomic<RetirePtr*> retired_ptrs{};
    // ???
};

}  // namespace NComponents::NHazard
