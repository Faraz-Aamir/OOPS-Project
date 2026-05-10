// DynamicArray.h — Templated resizable array (replaces std::vector)
// No STL containers used anywhere
// Faraz Aamir   | 25I-2069 | OOP Project | Spring 2026
// Shawwal       | 25I-2182 |
// Farhan        | 25I-2073 |

#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

#include <iostream>

template <typename T>
class DynamicArray {
private:
    T* data;          // Pointer to the underlying array
    int len;          // Number of elements currently stored
    int cap;          // Total allocated capacity

    // Double the capacity when full
    void grow() {
        int newCap = (cap == 0) ? 4 : cap * 2;
        T* newData = new T[newCap];
        for (int i = 0; i < len; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        cap = newCap;
    }

public:
    // --- Constructors & Destructor ---

    DynamicArray() : data(nullptr), len(0), cap(0) {}

    DynamicArray(int initialCapacity) : data(nullptr), len(0), cap(initialCapacity) {
        if (cap > 0) {
            data = new T[cap];
        }
    }

    // Copy constructor — deep copy
    DynamicArray(const DynamicArray& other) : data(nullptr), len(other.len), cap(other.cap) {
        if (cap > 0) {
            data = new T[cap];
            for (int i = 0; i < len; i++) {
                data[i] = other.data[i];
            }
        }
    }

    // Assignment operator — deep copy
    DynamicArray& operator=(const DynamicArray& other) {
        if (this == &other) return *this;

        delete[] data;
        len = other.len;
        cap = other.cap;
        data = nullptr;
        if (cap > 0) {
            data = new T[cap];
            for (int i = 0; i < len; i++) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    ~DynamicArray() {
        delete[] data;
        data = nullptr;
    }

    // --- Capacity ---

    int size() const { return len; }
    int capacity() const { return cap; }
    bool empty() const { return len == 0; }

    void reserve(int newCap) {
        if (newCap <= cap) return;
        T* newData = new T[newCap];
        for (int i = 0; i < len; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        cap = newCap;
    }

    // --- Element Access ---

    T& operator[](int index) { return data[index]; }
    const T& operator[](int index) const { return data[index]; }

    T& at(int index) {
        if (index < 0 || index >= len) {
            std::cerr << "DynamicArray::at() — index " << index
                      << " out of range [0, " << len << ")" << std::endl;
        }
        return data[index];
    }

    const T& at(int index) const {
        if (index < 0 || index >= len) {
            std::cerr << "DynamicArray::at() — index " << index
                      << " out of range [0, " << len << ")" << std::endl;
        }
        return data[index];
    }

    T& front() { return data[0]; }
    const T& front() const { return data[0]; }
    T& back() { return data[len - 1]; }
    const T& back() const { return data[len - 1]; }

    // Raw pointer for iteration compatibility
    T* getData() { return data; }
    const T* getData() const { return data; }

    // --- Modifiers ---

    void push_back(const T& value) {
        if (len >= cap) grow();
        data[len++] = value;
    }

    void pop_back() {
        if (len > 0) len--;
    }

    // Insert element at position (shifts elements right)
    void insert(int pos, const T& value) {
        if (pos < 0 || pos > len) return;
        if (len >= cap) grow();
        // Shift elements right
        for (int i = len; i > pos; i--) {
            data[i] = data[i - 1];
        }
        data[pos] = value;
        len++;
    }

    // Erase element at position (shifts elements left)
    void erase(int pos) {
        if (pos < 0 || pos >= len) return;
        for (int i = pos; i < len - 1; i++) {
            data[i] = data[i + 1];
        }
        len--;
    }

    void clear() {
        len = 0;
    }

    // --- Search ---

    // Find index of first occurrence, or -1 if not found
    int find(const T& value) const {
        for (int i = 0; i < len; i++) {
            if (data[i] == value) return i;
        }
        return -1;
    }

    bool contains(const T& value) const {
        return find(value) != -1;
    }

    // Remove first occurrence of value
    bool remove(const T& value) {
        int idx = find(value);
        if (idx == -1) return false;
        erase(idx);
        return true;
    }

    // --- Iteration support (begin/end pointers for range-based for loops) ---

    T* begin() { return data; }
    const T* begin() const { return data; }
    T* end() { return data + len; }
    const T* end() const { return data + len; }
};

#endif
