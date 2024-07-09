//
// Created by konstantin on 06.07.24.
//

#include "mutator.h"

#include <components/lock_free/hazard/thread_state.h>

namespace NComponents::NHazard {

Mutator::Mutator() { RegisterThread(); }

Mutator::~Mutator() {
    Release();
    UnregisterThread();
}

}  // namespace NComponents::NHazard
