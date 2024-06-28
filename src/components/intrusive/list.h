//
// Created by konstantin on 08.06.24.
//

#pragma once

#include <cassert>
#include <cstdint>

namespace NComponents {

template <class T>
class Node {
public:
    using TNode = Node<T>;

    TNode* Next() { return next_; }

    T* Cast() { return static_cast<T*>(this); }

    void LinkAfter(TNode* prev) { prev->next_ = this; }

private:
    TNode* next_{nullptr};
};

template <class T>
class IntrusiveList {
public:
    using Node = Node<T>;

    IntrusiveList() = default;
    ~IntrusiveList() noexcept { assert(IsEmpty()); }

    IntrusiveList(const IntrusiveList<T>& o)
        : size_(o.size_), head_(o.head_), tail_(o.tail_) {}

    bool IsEmpty() const { return 0 == size_; }

    void Clear() {
        head_ = nullptr;
        tail_ = nullptr;
    }

    void Push(Node* node) {
        assert(node);

        if (head_) {
            node->LinkAfter(tail_);
        } else {
            head_ = node;
        }
        tail_ = node;

        size_++;
    }

    T* Pop() {
        if (IsEmpty()) {
            return nullptr;
        }

        Node* p = head_;

        if (1 == size_) {
            head_ = nullptr;
            tail_ = nullptr;
        } else {
            head_ = p->Next();
        }

        size_--;

        return p->Cast();
    }

    std::size_t Size() const { return size_; }

private:
    std::size_t size_{0};
    Node* head_{nullptr};
    Node* tail_{nullptr};
};

}  // namespace NComponents
