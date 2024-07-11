//
// Created by konstantin on 14.05.24.
//

#pragma once

namespace NFibers::ctx {

class Runner {
public:
    virtual ~Runner() = default;

    virtual void Run() = 0;
};

}  // namespace fibers::ctx
