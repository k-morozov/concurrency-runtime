//
// Created by konstantin on 19.07.24.
//
#include "event.h"

namespace NComponents {

MutexAwaiter Event::operator co_await() { return MutexAwaiter{*this}; }

}  // namespace NComponents