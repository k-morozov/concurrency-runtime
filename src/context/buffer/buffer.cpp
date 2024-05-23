//
// Created by konstantin on 14.05.24.
//

#include "buffer.h"

namespace go::impl::ctx {

Buffer::Buffer(Buffer::Alloc alloc) : alloc_(alloc) {}

Buffer::~Buffer() { delete[] alloc_.data; }

}  // namespace go::impl::ctx
