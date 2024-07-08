//
// Created by konstantin on 08.07.24.
//

#pragma once

#include <atomic>

namespace NComponents::NHazard {

struct RetirePtr;

inline std::atomic<RetirePtr*> free_list = nullptr;
inline std::atomic<size_t> approximate_free_list_size{0};

void ScanFreeList();

}  // namespace NComponents::NHazard
