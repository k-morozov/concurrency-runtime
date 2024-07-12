//
// Created by konstantin on 14.05.24.
//

#pragma once

#include <context/buffer/buffer.h>
#include <context/runner.h>

namespace NFibers::NContext {

class BufferContext final {
public:
    void Setup(BufferView target_buffer, Runner* runner);
    void SwitchTo(BufferContext& target);

private:
    void* rsp_;
};

}  // namespace NFibers::NContext