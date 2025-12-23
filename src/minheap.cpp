#include "minheap.h"
#include <iostream>
#include <stdexcept>

MinHeap::MinHeap(int capacity)
        : size(0), capacity(capacity) {
    data = new HeapNode[capacity];
}

MinHeap::~MinHeap() {
    delete[] data;
}

bool MinHeap::isEmpty() const {
    return size == 0;
}

void MinHeap::insert(const HeapNode& node) {
    if (size == capacity) {
        throw std::runtime_error("Heap overflow");
    }
    data[size] = node;
    heapifyUp(size);
    size++;
}

HeapNode MinHeap::extractMin() {
    if (isEmpty()) {
        throw std::runtime_error("Heap is empty");
    }
    HeapNode min = data[0];
    data[0] = data[size - 1];
    size--;
    heapifyDown(0);
    return min;
}

void MinHeap::decreaseKey(int index, int newPriority) {
    if (index < 0 || index >= size) {
        throw std::runtime_error("Invalid index");
    }
    if (newPriority > data[index].priority) {
        throw std::runtime_error("New priority is greater than current");
    }
    data[index].priority = newPriority;
    heapifyUp(index);
}

void MinHeap::heapifyUp(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (data[index].priority >= data[parent].priority)
            break;
        std::swap(data[index], data[parent]);
        index = parent;
    }
}

void MinHeap::heapifyDown(int index) {
    while (true) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int smallest = index;

        if (left < size && data[left].priority < data[smallest].priority)
            smallest = left;
        if (right < size && data[right].priority < data[smallest].priority)
            smallest = right;

        if (smallest == index)
            break;

        std::swap(data[index], data[smallest]);
        index = smallest;
    }
}

void MinHeap::print() const {
    std::cout << "Heap: ";
    for (int i = 0; i < size; ++i) {
        std::cout << "(" << data[i].value << "," << data[i].priority << ") ";
    }
    std::cout << std::endl;
}