//
// Created by konstantin on 10.06.24.
//

#pragma once

#include <context/buffer/buffer.h>

namespace NFibers::NContext {

class SanitizerContext {
public:
    void Setup(BufferView /*buffer*/) {}

    void AfterStart() {}

    __attribute__((always_inline)) inline void BeforeSwitch(
        SanitizerContext& /*target*/) {}

    void AfterSwitch() {}

    __attribute__((always_inline)) inline void BeforeExit(
        SanitizerContext& /*target*/) {}
};

}  // namespace NFibers::NContext