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
        T value{};
        std::atomic<NodePointer> next;
    };

private:
    std::atomic<NodePointer> head;
    std::atomic<NodePointer> tail;

public:
    LockFreeQueue() {
        Node* dummy = new Node();
        NodePointer p{dummy, 0};
        head = p;
        tail = p;
    }
    ~LockFreeQueue() {

    }

    void Push(T item) {
        Node* new_node = new Node();
        new_node->value = std::move(item);
//        new_node->next.ptr = nullptr;

        NodePointer old_tail{};
        NodePointer old_next{};

        while(true) {
            old_tail = tail.load();
            old_next = old_tail.ptr->next.load();

            if (old_tail == tail.load()) {
                if (old_next.ptr == nullptr) {
                    if (tail.load().ptr->next.compare_exchange_strong(old_next, {new_node, old_next.counter + 1})) {
                        break;
                    }
                } else {
                    tail.compare_exchange_strong(old_tail, {old_next.ptr, old_tail.counter + 1});
                }
            }
        }
        tail.compare_exchange_strong(old_tail, {old_next.ptr, old_tail.counter + 1});
    }

    std::optional<T> TryPop() {
        std::optional<T> result;

//        NodePointer old_head{};
//        NodePointer old_tail{};
//
//        while (true) {
//            old_head = head.load();
//            old_tail = tail.load();
//            NodePointer next = old_head.ptr->next;
//
//            if (old_head == head.load()) {
//                if (old_head.ptr == old_tail.ptr) {
//                    if (next.ptr == nullptr) {
//                        return {};
//                    }
//                    tail.compare_exchange_strong(old_tail, {next.ptr, old_tail.counter + 1});
//                } else {
//                    result.emplace(std::move(next.ptr->value));
//                    if (head.compare_exchange_strong(old_head, {next.ptr, old_head.counter + 1})) {
//                        break;
//                    }
//                }
//            }
//        }
//
//        delete old_head.ptr;
        return result;
    }
};

}  // namespace NComponents
