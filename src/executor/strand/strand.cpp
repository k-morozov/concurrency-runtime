//
// Created by konstantin on 17.06.24.
//

#include "strand.h"

namespace executors {

Strand::Strand(IExecutor& underlying_) : underlying(underlying_) {}

void Strand::Submit(TaskPtr /*task*/) {}

}  // namespace executors