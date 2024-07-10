//
// Created by konstantin on 08.07.24.
//

#pragma once

#include <atomic>

namespace NComponents::NHazard {

struct RetirePtr;

struct ThreadState {
    std::atomic<void*> protected_ptr{};
    std::atomic<RetirePtr*> retired_ptrs{};
    std::atomic<bool> has_mutator{false};
};

}  // namespace NComponents::NHazard
