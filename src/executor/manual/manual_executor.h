//
// Created by konstantin on 04.06.24.
//

#pragma once

#include <executor/executor.h>

namespace pool {

class ManualExecutor final : public IExecutor {
public:
    void Submit(TaskPtr /*task*/) override {}
};

}  // namespace pool
