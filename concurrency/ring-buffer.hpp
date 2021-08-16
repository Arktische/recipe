#pragma once
#ifndef RING_BUFFER_H_
#define RING_BUFFER_H_
#include <atomic>
#include <cstddef>
namespace cpputil {
#define CACHE_LINE_SIZE 64
#define CONCAT_(a, b) a##b
#define CONCAT(a, b) CONCAT_(a, b)
#define CACHE_LINE_PADDING(size) char CONCAT(__PADDING__, __LINE__)[size]

template <typename T>
// lock-free mulitiple producer-consumer queue, linked-list implemention
class RingBuffer {
  public:
    explicit RingBuffer(size_t capacity) {
        capacity_mask_ = capacity - 1;
        for (size_t i = 1; i <= sizeof(void *) * 4; i <<= 1)
            capacity_mask_ |= capacity_mask_ >> i;
        capacity_ = capacity_mask_ + 1;

        queue_ = (Node *)new char[sizeof(Node) * capacity_];
        for (size_t i = 0; i < capacity_; ++i) {
            queue_[i].tail.store(i, std::memory_order_relaxed);
            queue_[i].head.store(-1, std::memory_order_relaxed);
        }

        tail_.store(0, std::memory_order_relaxed);
        head_.store(0, std::memory_order_relaxed);
    }

    ~RingBuffer() {
        for (size_t i = head_; i != tail_; ++i)
            (&queue_[i & capacity_mask_].data)->~T();

        delete[](char *) queue_;
    }

    size_t capacity() const { return capacity_; }

    size_t size() const {
        size_t head = head_.load(std::memory_order_acquire);
        return tail_.load(std::memory_order_relaxed) - head;
    }

    bool push(const T &data) {
        Node *node;
        size_t tail = tail_.load(std::memory_order_relaxed);
        for (;;) {
            node = &queue_[tail & capacity_mask_];
            if (node->tail.load(std::memory_order_relaxed) != tail)
                return false;
            if ((tail_.compare_exchange_weak(tail, tail + 1,
                                             std::memory_order_relaxed)))
                break;
        }
        new (&node->data) T(data);
        node->head.store(tail, std::memory_order_release);
        return true;
    }

    bool pop(T &result) {
        Node *node;
        size_t head = head_.load(std::memory_order_relaxed);
        for (;;) {
            node = &queue_[head & capacity_mask_];
            if (node->head.load(std::memory_order_relaxed) != head)
                return false;
            if (head_.compare_exchange_weak(head, head + 1,
                                            std::memory_order_relaxed))
                break;
        }
        result = node->data;
        (&node->data)->~T();
        node->tail.store(head + capacity_, std::memory_order_release);
        return true;
    }

  private:
    struct Node {
        T data;
        std::atomic<size_t> tail;
        std::atomic<size_t> head;
    };

  private:
    size_t capacity_mask_;
    Node *queue_;
    size_t capacity_;
    CACHE_LINE_PADDING(CACHE_LINE_SIZE);
    std::atomic<size_t> tail_;
    CACHE_LINE_PADDING(CACHE_LINE_SIZE);
    std::atomic<size_t> head_;
    CACHE_LINE_PADDING(CACHE_LINE_SIZE);
};
} // namespace cpputil
#endif
