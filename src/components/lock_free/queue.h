//
// Created by konstantin on 01.07.24.
//

#pragma once

#include <atomic>
#include <optional>

namespace NComponents {

/**
 * Simple, Fast, and Practical Non-Blocking and Blocking
 * Concurrent Queue Algorithms
 */
template <class T>
class LockFreeQueue final {
    struct Node;

    struct NodePointer {
        Node* ptr{};
        size_t counter{};

        bool operator==(const NodePointer& rhs) const {
            return this->ptr == rhs.ptr && this->counter == rhs.counter;
        }
    };

    struct Node {
        explicit Node(T v) : value(std::move(v)), next{{nullptr, 0}} {}
        T value{};
        std::atomic<NodePointer> next;
    };

private:
    std::atomic<NodePointer> head;
    std::atomic<NodePointer> tail;

public:
    LockFreeQueue() {
        Node* dummy = new Node(T{});
        NodePointer p{dummy, 0};
        head.store(p);
        tail.store(p);
    }
    ~LockFreeQueue() {
        while(auto n = TryPop()) {}
        auto p = head.load();
        delete p.ptr;
    }

    void Push(T item) {
        Node* new_node = new Node(std::move(item));

        NodePointer old_tail{};
        NodePointer old_next{};

        while(true) {
            old_tail = tail.load();
            old_next = old_tail.ptr->next.load();

            if (old_tail == tail.load()) {
                if (old_next.ptr == nullptr) {
                    if (old_tail.ptr->next.compare_exchange_strong(old_next, {new_node, old_next.counter + 1})) {
                        break;
                    }
                } else {
                    tail.compare_exchange_strong(old_tail, {old_next.ptr, old_tail.counter + 1});
                }
            }
        }
        tail.compare_exchange_strong(old_tail, {new_node, old_tail.counter + 1});
    }

    std::optional<T> TryPop() {
        std::optional<T> result;

        NodePointer old_head{};
        NodePointer old_tail{};

        while (true) {
            old_head = head.load();
            old_tail = tail.load();
            NodePointer next = old_head.ptr->next;

            if (old_head == head.load()) {
                if (old_head.ptr == old_tail.ptr) {
                    if (next.ptr == nullptr) {
                        return {};
                    }
                    tail.compare_exchange_strong(old_tail, {next.ptr, old_tail.counter + 1});
                } else {
                    result.emplace(next.ptr->value);
                    if (head.compare_exchange_strong(old_head, {next.ptr, old_head.counter + 1})) {
                        break;
                    }
                }
            }
        }

        delete old_head.ptr;
        return result;
    }
};

}  // namespace NComponents
