//
// Created by konstantin on 09.07.24.
//

#include "manager.h"

#include <components/lock_free/hazard/mutator.h>

namespace NComponents::NHazard {

Manager* Manager::Get() {
    static Manager gc;
    return &gc;
}

Mutator Manager::MakeMutator() {
    return Mutator(this);
}

}  // namespace NComponents::NHazard