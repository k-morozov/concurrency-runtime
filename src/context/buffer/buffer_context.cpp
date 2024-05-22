//
// Created by konstantin on 14.05.24.
//

#include "buffer_context.h"

#include "prepare_context.h"

namespace go::impl::ctx {

namespace {

// https://eli.thegreenplace.net/2011/09/06/stack-frame-layout-on-x86-64/
void CallRunner(void*, void*, void*, void*, void*, void*, void* arg7) {
    auto* t = (Runner*)arg7;
    t->Run();
}

}  // namespace

void BufferContext::Setup(BufferView target_buffer, Runner* runner) {
    // @TODO https://man7.org/linux/man-pages/man2/getcontext.2.html

    rsp_ = PrepareContextInBuffer((void*)&target_buffer.back(),
                                  (void*)CallRunner, (void*)runner);
}

void BufferContext::SwitchTo(BufferContext& target) {
    SwitchBufferContext(&rsp_, &target.rsp_);
}

}  // namespace go::impl::ctx
