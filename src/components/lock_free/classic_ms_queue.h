//
// Created by konstantin on 01.07.24.
//

#pragma once

#include <atomic>
#include <optional>

namespace NComponents {

/**
 * Michael-Scott Queue
 *
 * The algorithm implements the queue as a singly-linked list with
 * Head and Tail pointers. As in Valois’s algorithm, Head
 * always points to a dummy node, which is the first node in
 * the list. Tail points to either the last or second to last node
 * in the list. The algorithm uses compare and swap, with
 * modification counters to avoid the ABA problem.
 *
 * dummy -> node1 -> node2
 * head -> dummy
 * tail -> node2
 *
 * ref:
 * https://www.cs.rochester.edu/~scott/papers/1996_PODC_queues.pdf
 * https://cs.brown.edu/~mph/HerlihyW90/p463-herlihy.pdf
 * https://www.1024cores.net/home/lock-free-algorithms/eventcounts
 *
 */
template <class T>
class ClassicMSQueue final {
    struct Node;

    struct NodePointer {
        Node* node_ptr{};
        size_t counter{};

        bool operator==(const NodePointer& rhs) const {
            return this->node_ptr == rhs.node_ptr && this->counter == rhs.counter;
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
    ClassicMSQueue() {
        Node* dummy = new Node(T{});
        NodePointer p{dummy, 0};
        head.store(p);
        tail.store(p);
    }
    ~ClassicMSQueue() {
        while (head.load().node_ptr != nullptr) {
            Node* to_delete = head.load().node_ptr;
            head.store(to_delete->next);
            delete to_delete;
        }
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
            next_after_old_tail = old_tail.node_ptr->next.load();

            // Are tail and next consistent?
            if (old_tail == tail.load()) {
                // Was Tail pointing to the last node?
                if (next_after_old_tail.node_ptr == nullptr) {
                    // Try to link node at the end of the linked list
                    if (old_tail.node_ptr->next.compare_exchange_strong(
                            next_after_old_tail,
                            {new_node, next_after_old_tail.counter + 1})) {
                        break;
                    }
                } else {
                    // Tail was not pointing to the last node
                    // Try to swing Tail to the next node
                    tail.compare_exchange_strong(
                        old_tail,
                        {next_after_old_tail.node_ptr, old_tail.counter + 1});
                }
            }
        }
        // Try to swing Tail to the inserted node
        tail.compare_exchange_strong(old_tail,
                                     {new_node, old_tail.counter + 1});
    }

    std::optional<T> TryPop() {
        std::optional<T> result{};

        NodePointer old_head{};
        NodePointer old_tail{};

        while (true) {
            old_head = head.load();
            old_tail = tail.load();
            NodePointer next_after_old_head = old_head.node_ptr->next.load();

            // Are head, tail, and next consistent?
            if (old_head == head.load()) {
                //  Is queue empty or Tail falling behind?
                if (old_head.node_ptr == old_tail.node_ptr) {
                    // Is queue empty?
                    if (next_after_old_head.node_ptr == nullptr) {
                        return {};
                    }

                    // Tail is falling behind. Try to advance it
                    tail.compare_exchange_strong(
                        old_tail,
                        {next_after_old_head.node_ptr, old_tail.counter + 1});

                } else {
                    // Read value before CAS, otherwise another dequeue might
                    // free the next node
                    result.emplace(next_after_old_head.node_ptr->value);

                    // Try to swing Head to the next node
                    if (head.compare_exchange_strong(
                            old_head,
                            {next_after_old_head.node_ptr, old_head.counter + 1})) {
                        break;
                    }
                }
            }
        }
        // It is safe now to free the old dummy node
        delete old_head.node_ptr;
        return result;
    }
};


template <class T>
class SimpleMSQueue final {
    struct Node {
        std::optional<T> value;
        std::atomic<Node*> next{nullptr};
    };
private:
    std::atomic<Node*> head;
    std::atomic<Node*> tail;
public:
    SimpleMSQueue() {
        Node* dummy = new Node{};
        head.store(dummy);
        tail.store(dummy);
    }

    ~SimpleMSQueue() {
        while (head != nullptr) {
            Node* to_delete = head.load();
            head.store(to_delete->next);
            delete to_delete;
        }
    }

    void Push(T item) {
        Node* new_node = new Node{std::move(item), nullptr};
        Node* old_tail{};

        while(true) {
            old_tail = tail.load();

            if (old_tail->next.load() != nullptr) {
                // concurrent push
                // helping
                tail.compare_exchange_weak(old_tail, old_tail->next);
                continue;
            }

            Node* null_ptr = nullptr;
            if (old_tail->next.compare_exchange_weak(null_ptr, new_node)) {
                break;
            }
        }
        tail.compare_exchange_strong(old_tail, new_node);
    }

    std::optional<T> TryPop() {
        while (true) {
            Node* old_head = head.load();

            if (old_head->next.load() == nullptr) {
                return {};
            }

            Node* old_tail = tail.load();
            if (old_head == old_tail) {
                // helping
                tail.compare_exchange_weak(old_tail, old_tail->next);
                continue;
            }

            if (head.compare_exchange_weak(old_head, old_head->next)) {
                Node* next = old_head->next;
                T result = std::move(*next->value);
                return result;
            }
        }
    }
};

}  // namespace NComponents
