//
// Created by konstantin on 14.05.24.
//

#pragma once

#include <span>

namespace go::impl::ctx {

using BufferView = std::span<std::byte>;

class ContextBuffer {
public:
    ~ContextBuffer();

    static ContextBuffer AllocBytes(const size_t bytes) {
        return ContextBuffer(Alloc{new std::byte[bytes], bytes});
    }

    BufferView GetSpan() const { return {alloc_.data, alloc_.size}; }

private:
    struct Alloc {
        std::byte* data;
        size_t size;
    };

    Alloc alloc_;

private:
    explicit ContextBuffer(Alloc alloc);
};

}  // namespace go::impl::ctx
