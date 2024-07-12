//
// Created by konstantin on 10.06.24.
//

#pragma once

#include <context/buffer/buffer.h>
#include <sanitizer/tsan_interface.h>

namespace NFibers::NContext {

class SanitizerContext final {
public:
    void Setup(BufferView /*buffer*/) { fiber_ = __tsan_create_fiber(0); }

    void AfterStart() { After(); }

    __attribute__((always_inline)) inline void BeforeSwitch(
        SanitizerContext& target) {
        fiber_ = __tsan_get_current_fiber();
        __tsan_switch_to_fiber(target.fiber_, 0);
    }

    void AfterSwitch() { After(); }

    __attribute__((always_inline)) inline void BeforeExit(
        SanitizerContext& target) {
        target.exit_from_ = this;
        __tsan_switch_to_fiber(target.fiber_, 0);
    }

private:
    void* fiber_{};
    SanitizerContext* exit_from_{nullptr};

    void After() {
        if (exit_from_ != nullptr) {
            __tsan_destroy_fiber(exit_from_->fiber_);
            exit_from_ = nullptr;
        }
    }
};

}  // namespace NFibers::NContext