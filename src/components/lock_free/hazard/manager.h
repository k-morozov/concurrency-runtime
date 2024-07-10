//
// Created by konstantin on 09.07.24.
//

#pragma once

#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <unordered_set>

#include <components/lock_free/hazard/mutator.h>

namespace NComponents::NHazard {

struct ThreadState;

class Manager final {
    friend class Mutator;

public:
    ~Manager();
    static Manager* Get();
    Mutator MakeMutator();

protected:
    std::mutex thread_lock;
    std::unordered_map<std::thread::id, ThreadState*> threads;
    std::mutex scan_lock;

    std::atomic<size_t> approximate_free_list_size{0};

    void Collect();

private:
    Manager() = default;
};

}  // namespace NComponents::NHazard