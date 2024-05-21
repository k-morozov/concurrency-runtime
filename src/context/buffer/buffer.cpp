//
// Created by konstantin on 14.05.24.
//

#include "buffer.h"

namespace go::impl::ctx {

ContextBuffer::ContextBuffer(ContextBuffer::Alloc alloc) : alloc_(alloc) {}

ContextBuffer::~ContextBuffer() { delete[] alloc_.data; }

}  // namespace go::impl::ctx
