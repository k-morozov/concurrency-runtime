//
// Created by konstantin on 08.07.24.
//

#pragma once

#include <atomic>
#include <mutex>
#include <unordered_set>

namespace NComponents::NHazard {

struct ThreadState {
    std::atomic<void*>* ptr{};
};

inline std::mutex thread_lock;
inline std::unordered_set<ThreadState*> threads;
inline thread_local std::atomic<void*> hazard_ptr{};

void RegisterThread();
void UnregisterThread();

}  // namespace NComponents::NHazard
