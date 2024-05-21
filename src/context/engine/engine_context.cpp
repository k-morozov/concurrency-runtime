//
// Created by konstantin on 14.05.24.
//

#include "engine_context.h"

#include <internal/internal_context.h>

namespace go::impl::ctx {

namespace {

// https://eli.thegreenplace.net/2011/09/06/stack-frame-layout-on-x86-64/
void EngineContextRunner(void*, void*, void*, void*, void*, void*, void* arg7) {
    auto* t = (Runner*)arg7;
    t->Run();
}

}  // namespace

void EngineContext::Setup(BufferView target_buffer, Runner* runner) {
    // @TODO https://man7.org/linux/man-pages/man2/getcontext.2.html

    rsp_ = SetupEngineContext((void*)&target_buffer.back(),
                              (void*)EngineContextRunner, (void*)runner);
}

void EngineContext::SwitchTo(EngineContext& /*target*/) {}

}  // namespace go::impl::ctx
