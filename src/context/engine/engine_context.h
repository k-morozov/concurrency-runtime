//
// Created by konstantin on 14.05.24.
//

#pragma once

#include <runner.h>

#include <buffer/buffer.h>

namespace go::impl::ctx {

class EngineContext final {
public:
    void Setup(BufferView target_buffer, Runner* runner);
    void SwitchTo(EngineContext& target);

private:
    void* rsp_;
};

}  // namespace go::impl::ctx