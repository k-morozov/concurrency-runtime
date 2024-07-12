//
// Created by konstantin on 14.05.24.
//

#include "buffer.h"

#include <utility>

namespace NFibers::NContext {

Buffer::Buffer(Buffer::Alloc alloc) : alloc_(alloc) {}

Buffer::Buffer(Buffer&& buffer) noexcept
    : alloc_(std::exchange(buffer.alloc_, {nullptr, 0})) {}

Buffer::~Buffer() { delete[] alloc_.data; }

}  // namespace NFibers::NContext
