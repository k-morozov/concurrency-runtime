//
// Created by konstantin on 14.05.24.
//

#pragma once

#include <span>

namespace go::impl::ctx {

using BufferView = std::span<std::byte>;

class Buffer {
public:
    ~Buffer();

    Buffer(const Buffer&) noexcept = delete;
    Buffer(Buffer&&) noexcept;

    static Buffer AllocBytes(const size_t bytes) {
        return Buffer(Alloc{new std::byte[bytes], bytes});
    }

    [[nodiscard]] BufferView GetSpan() const {
        return {alloc_.data, alloc_.size};
    }

private:
    struct Alloc {
        std::byte* data;
        size_t size;
    };

    Alloc alloc_;

private:
    explicit Buffer(Alloc alloc);
};

}  // namespace go::impl::ctx
