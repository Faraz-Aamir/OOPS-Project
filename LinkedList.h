// LinkedList.h — Doubly-linked list (replaces std::list)
// No STL containers used anywhere
// Faraz Aamir | 25I-2069 | OOP Project | Spring 2026

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>

template <typename T>
class LinkedList {
public:
    // Node structure for doubly-linked list
    struct Node {
        T data;
        Node* next;
        Node* prev;
        Node(const T& val) : data(val), next(nullptr), prev(nullptr) {}
    };

private:
    Node* head;
    Node* tail;
    int len;

public:
    // --- Constructors & Destructor ---

    LinkedList() : head(nullptr), tail(nullptr), len(0) {}

    // Copy constructor
    LinkedList(const LinkedList& other) : head(nullptr), tail(nullptr), len(0) {
        Node* current = other.head;
        while (current) {
            push_back(current->data);
            current = current->next;
        }
    }

    // Assignment operator
    LinkedList& operator=(const LinkedList& other) {
        if (this == &other) return *this;
        clear();
        Node* current = other.head;
        while (current) {
            push_back(current->data);
            current = current->next;
        }
        return *this;
    }

    ~LinkedList() {
        clear();
    }

    // --- Capacity ---
    int size() const { return len; }
    bool empty() const { return len == 0; }

    // --- Element Access ---
    T& front() { return head->data; }
    const T& front() const { return head->data; }
    T& back() { return tail->data; }
    const T& back() const { return tail->data; }

    // Access by index (O(n))
    T& at(int index) {
        Node* current = head;
        for (int i = 0; i < index; i++) current = current->next;
        return current->data;
    }

    const T& at(int index) const {
        Node* current = head;
        for (int i = 0; i < index; i++) current = current->next;
        return current->data;
    }

    // --- Modifiers ---

    void push_back(const T& value) {
        Node* node = new Node(value);
        if (!tail) {
            head = tail = node;
        } else {
            node->prev = tail;
            tail->next = node;
            tail = node;
        }
        len++;
    }

    void push_front(const T& value) {
        Node* node = new Node(value);
        if (!head) {
            head = tail = node;
        } else {
            node->next = head;
            head->prev = node;
            head = node;
        }
        len++;
    }

    void pop_back() {
        if (!tail) return;
        Node* toDelete = tail;
        tail = tail->prev;
        if (tail) tail->next = nullptr;
        else head = nullptr;
        delete toDelete;
        len--;
    }

    void pop_front() {
        if (!head) return;
        Node* toDelete = head;
        head = head->next;
        if (head) head->prev = nullptr;
        else tail = nullptr;
        delete toDelete;
        len--;
    }

    // Remove a specific node (O(1) if you have the node pointer)
    void removeNode(Node* node) {
        if (!node) return;
        if (node->prev) node->prev->next = node->next;
        else head = node->next;
        if (node->next) node->next->prev = node->prev;
        else tail = node->prev;
        delete node;
        len--;
    }

    // Remove first occurrence of value
    bool remove(const T& value) {
        Node* current = head;
        while (current) {
            if (current->data == value) {
                removeNode(current);
                return true;
            }
            current = current->next;
        }
        return false;
    }

    void clear() {
        Node* current = head;
        while (current) {
            Node* next = current->next;
            delete current;
            current = next;
        }
        head = tail = nullptr;
        len = 0;
    }

    // --- Search ---

    Node* find(const T& value) {
        Node* current = head;
        while (current) {
            if (current->data == value) return current;
            current = current->next;
        }
        return nullptr;
    }

    bool contains(const T& value) const {
        Node* current = head;
        while (current) {
            if (current->data == value) return true;
            current = current->next;
        }
        return false;
    }

    // --- Iteration ---

    Node* getHead() { return head; }
    const Node* getHead() const { return head; }
    Node* getTail() { return tail; }
    const Node* getTail() const { return tail; }

    // Simple iterator for range-based for loops
    class Iterator {
    private:
        Node* current;
    public:
        Iterator(Node* node) : current(node) {}
        T& operator*() { return current->data; }
        Iterator& operator++() { current = current->next; return *this; }
        bool operator!=(const Iterator& other) const { return current != other.current; }
    };

    class ConstIterator {
    private:
        const Node* current;
    public:
        ConstIterator(const Node* node) : current(node) {}
        const T& operator*() const { return current->data; }
        ConstIterator& operator++() { current = current->next; return *this; }
        bool operator!=(const ConstIterator& other) const { return current != other.current; }
    };

    Iterator begin() { return Iterator(head); }
    Iterator end() { return Iterator(nullptr); }
    ConstIterator begin() const { return ConstIterator(head); }
    ConstIterator end() const { return ConstIterator(nullptr); }
};

#endif
