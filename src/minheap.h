#ifndef MINHEAP_H
#define MINHEAP_H

#include <stdexcept>
#include <utility>

template <typename T>
class MinHeap {
private:
    T* data;
    int size;
    int capacity;

    // funkcja porównująca (priorytet)
    bool (*less)(const T&, const T&);

    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (!less(data[index], data[parent]))
                break;
            std::swap(data[index], data[parent]);
            index = parent;
        }
    }

    void heapifyDown(int index) {
        while (true) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int smallest = index;

            if (left < size && less(data[left], data[smallest]))
                smallest = left;
            if (right < size && less(data[right], data[smallest]))
                smallest = right;

            if (smallest == index)
                break;

            std::swap(data[index], data[smallest]);
            index = smallest;
        }
    }

public:
    MinHeap(int capacity, bool (*cmp)(const T&, const T&))
        : size(0), capacity(capacity), less(cmp) {
        data = new T[capacity];
    }

    ~MinHeap() {
        delete[] data;
    }

    [[nodiscard]] bool isEmpty() const {
        return size == 0;
    }

    void insert(const T& value) {
        if (size == capacity)
            throw std::runtime_error("Heap overflow");
        data[size] = value;
        heapifyUp(size);
        size++;
    }

    T extractMin() {
        if (isEmpty())
            throw std::runtime_error("Heap is empty");
        T min = data[0];
        data[0] = data[size - 1];
        size--;
        heapifyDown(0);
        return min;
    }

    int getSize() const {
        return size;
    }
};

#endif