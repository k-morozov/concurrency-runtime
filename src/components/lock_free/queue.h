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
        // Allocate a new node from the free list
        Node* new_node = new Node(std::move(item));

        NodePointer old_tail{};
        NodePointer next_after_old_tail{};

        while (true) {
            // Read Tail.ptr and Tail.count together
            old_tail = tail.load();

            // Read next ptr and count fields together
            next_after_old_tail = old_tail.ptr->next.load();

            // Are tail and next consistent?
            if (old_tail == tail.load()) {
                // Was Tail pointing to the last node?
                if (next_after_old_tail.ptr == nullptr) {
                    // Try to link node at the end of the linked list
                    if (old_tail.ptr->next.compare_exchange_strong(
                            next_after_old_tail,
                            {new_node, next_after_old_tail.counter + 1})) {
                        break;
                    }
                } else {
                    // Tail was not pointing to the last node
                    // Try to swing Tail to the next node
                    tail.compare_exchange_strong(
                        old_tail,
                        {next_after_old_tail.ptr, old_tail.counter + 1});
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
            NodePointer next_after_old_head = old_head.ptr->next;

            // Are head, tail, and next consistent?
            if (old_head == head.load()) {
                //  Is queue empty or Tail falling behind?
                if (old_head.ptr == old_tail.ptr) {
                    // Is queue empty?
                    if (next_after_old_head.ptr == nullptr) {
                        return {};
                    }

                    // Tail is falling behind. Try to advance it
                    tail.compare_exchange_strong(
                        old_tail,
                        {next_after_old_head.ptr, old_tail.counter + 1});

                } else {
                    // Read value before CAS, otherwise another dequeue might
                    // free the next node
                    result.emplace(next_after_old_head.ptr->value);

                    // Try to swing Head to the next node
                    if (head.compare_exchange_strong(
                            old_head,
                            {next_after_old_head.ptr, old_head.counter + 1})) {
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
