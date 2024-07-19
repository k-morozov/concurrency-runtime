//
// Created by konstantin on 19.07.24.
//

#include "mutex_awaiter.h"

namespace NComponents {

MutexAwaiter::MutexAwaiter(Event& event) : event(event) {}

}  // namespace NComponents