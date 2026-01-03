#ifndef MINHEAP_H
#define MINHEAP_H

#include <stdexcept>
#include <functional>

template <typename T>
class MinHeap {
private:
    T* data;
    int size;
    int capacity;
    bool (*less)(const T&, const T&);

    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (!less(data[index], data[parent])) break;
            std::swap(data[index], data[parent]);
            index = parent;
        }
    }

    void heapifyDown(int index) {
        while (true) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int smallest = index;

            if (left < size && less(data[left], data[smallest])) smallest = left;
            if (right < size && less(data[right], data[smallest])) smallest = right;

            if (smallest == index) break;

            std::swap(data[index], data[smallest]);
            index = smallest;
        }
    }

public:
    MinHeap(int capacity, bool (*cmp)(const T&, const T&))
        : data(nullptr), size(0), capacity(capacity), less(cmp) {
        if (capacity < 0) throw std::runtime_error("Invalid capacity");
        data = (capacity > 0) ? new T[capacity] : nullptr;
    }

    ~MinHeap() {
        delete[] data;
    }

    [[nodiscard]] bool isEmpty() const { return size == 0; }
    [[nodiscard]] int getSize() const { return size; }

    void insert(const T& value) {
        if (size == capacity) throw std::runtime_error("Heap overflow");
        data[size] = value;
        heapifyUp(size);
        size++;
    }

    T extractMin() {
        if (isEmpty()) throw std::runtime_error("Heap is empty");
        T min = data[0];
        data[0] = data[size - 1];
        size--;
        if (size > 0) heapifyDown(0);
        return min;
    }

    void buildFromArray(const T* arr, int n) {
        if (n < 0) throw std::runtime_error("Invalid n");
        if (n > capacity) throw std::runtime_error("Array larger than heap capacity");

        for (int i = 0; i < n; i++) data[i] = arr[i];
        size = n;

        for (int i = (size / 2) - 1; i >= 0; i--) {
            heapifyDown(i);
        }
    }

    void decreaseKey(int index, const T& newValue) {
        if (index < 0 || index >= size) throw std::runtime_error("Index out of range");
        if (!less(newValue, data[index])) {
            throw std::runtime_error("New value does not decrease priority");
        }
        data[index] = newValue;
        heapifyUp(index);
    }

    void debugPrint(const std::function<void(const T&)>& printer) const {
        for (int i = 0; i < size; i++) {
            printer(data[i]);
        }
    }
};
#endif