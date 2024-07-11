//
// Created by konstantin on 09.07.24.
//

#pragma once

#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <thread>
#include <unordered_set>

#include <components/lock_free/hazard/mutator.h>

namespace NComponents::NHazard {

struct ThreadState;

class HazardManager final {
    static constexpr size_t LimitFreeList = 8;

    friend class Mutator;

public:
    ~HazardManager();
    static HazardManager* Get();
    Mutator MakeMutator();

protected:
    std::mutex thread_lock;
    std::unordered_map<std::thread::id, ThreadState*> threads;

    std::atomic<size_t> approximate_free_list_size{0};

    std::optional<std::thread> collector_thread;
    std::atomic<bool> cancel_collect{};

    void Collect();

private:
    HazardManager();
};

}  // namespace NComponents::NHazard