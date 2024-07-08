//
// Created by konstantin on 01.07.24.
//

#pragma once

#include <atomic>
#include <optional>

#include <components/lock_free/hazard/hazard.h>
#include <components/lock_free/hazard/thread_state.h>

namespace NComponents {

/**
 * Michael-Scott Queue
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
        NComponents::NHazard::RegisterThread();
        while (true) {
            Node* old_head = NHazard::Acquire(&head);

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
                NHazard::Retire(old_head);
                NHazard::Release();
                NComponents::NHazard::UnregisterThread();
                return result;
            }
        }
    }
};

}  // namespace NComponents
