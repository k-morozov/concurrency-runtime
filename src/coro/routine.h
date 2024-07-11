//
// Created by konstantin on 01.06.24.
//

#pragma once

#include <components/function/basic_function.h>
#include <functional>

namespace NFibers::coro {

//using Routine = NComponents::BasicFunction<void()>;
using Routine = std::function<void()>;

}
