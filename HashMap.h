// HashMap.h — Key-value associative container using chaining (replaces std::map/std::unordered_map)
// No STL containers used anywhere
// Faraz Aamir | 25I-2069 | OOP Project | Spring 2026

#ifndef HASHMAP_H
#define HASHMAP_H

#include "MyString.h"
#include "DynamicArray.h"
#include <iostream>

// Generic hash function — must be specialized for each key type
template <typename K>
struct HashFunc {
    unsigned int operator()(const K& key) const {
        // Default: hash the raw bytes (works for primitive types)
        unsigned int h = 5381;
        const char* ptr = reinterpret_cast<const char*>(&key);
        for (unsigned int i = 0; i < sizeof(K); i++) {
            h = ((h << 5) + h) + (unsigned char)ptr[i];
        }
        return h;
    }
};

// Specialization for MyString
template <>
struct HashFunc<MyString> {
    unsigned int operator()(const MyString& key) const {
        return key.hash();
    }
};

// Specialization for int
template <>
struct HashFunc<int> {
    unsigned int operator()(const int& key) const {
        return (unsigned int)key;
    }
};

template <typename K, typename V>
class HashMap {
public:
    // A single key-value pair
    struct Entry {
        K key;
        V value;
        bool occupied; // Whether this entry is in use

        Entry() : occupied(false) {}
        Entry(const K& k, const V& v) : key(k), value(v), occupied(true) {}
    };

private:
    // Each bucket is a DynamicArray of entries (chaining for collision resolution)
    struct Bucket {
        DynamicArray<Entry> entries;
    };

    Bucket* buckets;       // Array of buckets
    int numBuckets;        // Number of buckets
    int numEntries;        // Total entries across all buckets
    HashFunc<K> hasher;    // Hash function

    // Resize when load factor exceeds threshold
    void rehash(int newBucketCount) {
        Bucket* oldBuckets = buckets;
        int oldNumBuckets = numBuckets;

        numBuckets = newBucketCount;
        buckets = new Bucket[numBuckets];
        numEntries = 0;

        // Re-insert all existing entries
        for (int i = 0; i < oldNumBuckets; i++) {
            for (int j = 0; j < oldBuckets[i].entries.size(); j++) {
                Entry& e = oldBuckets[i].entries[j];
                if (e.occupied) {
                    put(e.key, e.value);
                }
            }
        }

        delete[] oldBuckets;
    }

    int getBucketIndex(const K& key) const {
        return hasher(key) % numBuckets;
    }

public:
    // --- Constructors & Destructor ---

    HashMap(int initialBuckets = 16) : numBuckets(initialBuckets), numEntries(0) {
        buckets = new Bucket[numBuckets];
    }

    // Copy constructor
    HashMap(const HashMap& other) : numBuckets(other.numBuckets), numEntries(0) {
        buckets = new Bucket[numBuckets];
        for (int i = 0; i < other.numBuckets; i++) {
            for (int j = 0; j < other.buckets[i].entries.size(); j++) {
                const Entry& e = other.buckets[i].entries[j];
                if (e.occupied) {
                    put(e.key, e.value);
                }
            }
        }
    }

    // Assignment operator
    HashMap& operator=(const HashMap& other) {
        if (this == &other) return *this;
        delete[] buckets;
        numBuckets = other.numBuckets;
        numEntries = 0;
        buckets = new Bucket[numBuckets];
        for (int i = 0; i < other.numBuckets; i++) {
            for (int j = 0; j < other.buckets[i].entries.size(); j++) {
                const Entry& e = other.buckets[i].entries[j];
                if (e.occupied) {
                    put(e.key, e.value);
                }
            }
        }
        return *this;
    }

    ~HashMap() {
        delete[] buckets;
    }

    // --- Capacity ---
    int size() const { return numEntries; }
    bool empty() const { return numEntries == 0; }

    // --- Operations ---

    // Insert or update a key-value pair
    void put(const K& key, const V& value) {
        // Check load factor — rehash if > 0.75
        if (numEntries > numBuckets * 3 / 4) {
            rehash(numBuckets * 2);
        }

        int idx = getBucketIndex(key);
        DynamicArray<Entry>& chain = buckets[idx].entries;

        // Check if key already exists — update if so
        for (int i = 0; i < chain.size(); i++) {
            if (chain[i].occupied && chain[i].key == key) {
                chain[i].value = value;
                return;
            }
        }

        // Key doesn't exist — insert new entry
        chain.push_back(Entry(key, value));
        numEntries++;
    }

    // Get value by key — returns pointer (nullptr if not found)
    V* get(const K& key) {
        int idx = getBucketIndex(key);
        DynamicArray<Entry>& chain = buckets[idx].entries;
        for (int i = 0; i < chain.size(); i++) {
            if (chain[i].occupied && chain[i].key == key) {
                return &chain[i].value;
            }
        }
        return nullptr;
    }

    const V* get(const K& key) const {
        int idx = getBucketIndex(key);
        const DynamicArray<Entry>& chain = buckets[idx].entries;
        for (int i = 0; i < chain.size(); i++) {
            if (chain[i].occupied && chain[i].key == key) {
                return &chain[i].value;
            }
        }
        return nullptr;
    }

    // Check if key exists
    bool containsKey(const K& key) const {
        return get(key) != nullptr;
    }

    // Remove by key
    bool remove(const K& key) {
        int idx = getBucketIndex(key);
        DynamicArray<Entry>& chain = buckets[idx].entries;
        for (int i = 0; i < chain.size(); i++) {
            if (chain[i].occupied && chain[i].key == key) {
                chain.erase(i);
                numEntries--;
                return true;
            }
        }
        return false;
    }

    // Subscript operator — creates entry if not found
    V& operator[](const K& key) {
        V* val = get(key);
        if (val) return *val;
        put(key, V());
        return *get(key);
    }

    void clear() {
        for (int i = 0; i < numBuckets; i++) {
            buckets[i].entries.clear();
        }
        numEntries = 0;
    }

    // --- Get all keys ---
    DynamicArray<K> keys() const {
        DynamicArray<K> result;
        for (int i = 0; i < numBuckets; i++) {
            for (int j = 0; j < buckets[i].entries.size(); j++) {
                if (buckets[i].entries[j].occupied) {
                    result.push_back(buckets[i].entries[j].key);
                }
            }
        }
        return result;
    }

    // --- Get all values ---
    DynamicArray<V> values() const {
        DynamicArray<V> result;
        for (int i = 0; i < numBuckets; i++) {
            for (int j = 0; j < buckets[i].entries.size(); j++) {
                if (buckets[i].entries[j].occupied) {
                    result.push_back(buckets[i].entries[j].value);
                }
            }
        }
        return result;
    }

    // --- Iteration (for range-based for loops) ---
    // Returns all entries as a flat DynamicArray of Entry
    struct KeyValuePair {
        K key;
        V value;
    };

    DynamicArray<KeyValuePair> entries() const {
        DynamicArray<KeyValuePair> result;
        for (int i = 0; i < numBuckets; i++) {
            for (int j = 0; j < buckets[i].entries.size(); j++) {
                if (buckets[i].entries[j].occupied) {
                    KeyValuePair kv;
                    kv.key = buckets[i].entries[j].key;
                    kv.value = buckets[i].entries[j].value;
                    result.push_back(kv);
                }
            }
        }
        return result;
    }
};

#endif
