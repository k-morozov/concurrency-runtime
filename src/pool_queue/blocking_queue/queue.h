//
// Created by konstantin on 10.05.24.
//

#pragma once

#include <optional>

namespace pool {

// UnboundedBlockingQueue
template <class T>
class UnboundedBlockingQueue final {
public:
    bool Put(T) { return true; }
    std::optional<T> Take() { return std::nullopt; }
    void Close() {};
};

}  // namespace pool