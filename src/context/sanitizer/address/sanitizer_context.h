//
// Created by konstantin on 10.06.24.
//

#pragma once

#include <context/buffer/buffer.h>
#include <sanitizer/asan_interface.h>

namespace fibers::ctx {

class SanitizerContext {
public:
    void Setup(BufferView stack) {
        stack_ = stack.data();
        stack_size_ = stack.size();
    }

    void AfterStart() {
        __sanitizer_finish_switch_fiber(nullptr, &(from_->stack_),
                                        &(from_->stack_size_));
    }

    void BeforeSwitch(SanitizerContext& target) {
        target.from_ = this;
        __sanitizer_start_switch_fiber(&fake_stack_, target.stack_,
                                       target.stack_size_);
    }

    void AfterSwitch() {
        __sanitizer_finish_switch_fiber(fake_stack_, &(from_->stack_),
                                        &(from_->stack_size_));
    }

    void BeforeExit(SanitizerContext& target) {
        target.from_ = this;
        __sanitizer_start_switch_fiber(nullptr, target.stack_,
                                       target.stack_size_);
    }

private:
    const void* stack_;
    size_t stack_size_;
    void* fake_stack_;

    SanitizerContext* from_;
};

}  // namespace fibers::ctx