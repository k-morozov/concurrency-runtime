//
// Created by konstantin on 09.07.24.
//

#pragma once

#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <shared_mutex>

#include <components/lock_free/hazard/mutator.h>

namespace NComponents::NHazard {

class Manager final {
public:
    static Manager* Get();
    Mutator MakeMutator();
private:
    Manager() = default;
};

}  // namespace NComponents::NHazard