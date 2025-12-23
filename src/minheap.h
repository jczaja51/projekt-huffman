#ifndef MINHEAP_H
#define MINHEAP_H

struct HeapNode {
    char value;
    int priority;
};

class MinHeap {
private:
    HeapNode* data;
    int size;
    int capacity;

    void heapifyUp(int index);
    void heapifyDown(int index);

public:
    explicit MinHeap(int capacity);
    ~MinHeap();

    [[nodiscard]] bool isEmpty() const;

    void insert(const HeapNode& node);
    HeapNode extractMin();
    void decreaseKey(int index, int newPriority);

    void print() const;
};

#endif