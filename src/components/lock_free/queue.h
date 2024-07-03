//
// Created by konstantin on 01.07.24.
//

#pragma once

#include <atomic>
#include <optional>

namespace NComponents {

/**
 * Michael-Scott Queue
 * https://www.cs.rochester.edu/~scott/papers/1996_PODC_queues.pdf
 * The algorithm implements the queue as a singly-linked list with
 * Head and Tail pointers. As in Valois’s algorithm, Head
 * always points to a dummy node, which is the first node in
 * the list. Tail points to either the last or second to last node
 * in the list. The algorithm uses compare and swap, with
 * modification counters to avoid the ABA problem.
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
        while (auto n = TryPop()) {
        }
        auto p = head.load();
        delete p.ptr;
    }

    void Push(T item) {
        Node* new_node = new Node(std::move(item));

        NodePointer old_tail{};
        NodePointer old_next{};

        while (true) {
            old_tail = tail.load();
            old_next = old_tail.ptr->next.load();

            if (old_tail == tail.load()) {
                if (old_next.ptr == nullptr) {
                    if (old_tail.ptr->next.compare_exchange_strong(
                            old_next, {new_node, old_next.counter + 1})) {
                        break;
                    }
                } else {
                    // Tail was not pointing to the last node
                    // Try to swing Tail to the next node
                    tail.compare_exchange_strong(
                        old_tail, {old_next.ptr, old_tail.counter + 1});
                }
            }
        }
        // Try to swing Tail to the inserted node
        tail.compare_exchange_strong(old_tail,
                                     {new_node, old_tail.counter + 1});
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
                    tail.compare_exchange_strong(
                        old_tail, {next.ptr, old_tail.counter + 1});
                } else {
                    // Read value before CAS, otherwise another dequeue might
                    // free the next node
                    result.emplace(next.ptr->value);
                    // Try to swing Head to the next node
                    if (head.compare_exchange_strong(
                            old_head, {next.ptr, old_head.counter + 1})) {
                        break;
                    }
                }
            }
        }

        // It is safe now to free the old dummy node
        delete old_head.ptr;
        return result;
    }
};

}  // namespace NComponents
