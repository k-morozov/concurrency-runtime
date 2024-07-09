//
// Created by konstantin on 08.07.24.
//

#pragma once

#include <atomic>

namespace NComponents::NHazard {

struct ThreadState {
    std::atomic<void*>* thread_hazard_ptr{};
    // Protected pointers
    // Retired pointers
    // ???
};

}  // namespace NComponents::NHazard
