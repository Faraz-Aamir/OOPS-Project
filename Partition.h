// Partition.h — Logical disk partition
// Ported from Assignment 3 — no STL used
// Faraz Aamir | 25I-2069 | OOP Project | Spring 2026

#ifndef PARTITION_H
#define PARTITION_H

#include "MyString.h"
#include "Directory.h"
#include <iostream>

enum class FileSystemFormat { FAT32, NTFS, EXT4 };

class Partition {
private:
    MyString label;
    size_t allocatedCapacity;
    FileSystemFormat formatType;
    Directory* rootDirectory; // Composition: partition owns its root directory
    bool mounted;

public:
    Partition(const MyString& label, size_t capacity, FileSystemFormat format,
              User* rootOwner);
    ~Partition();

    Partition(const Partition&) = delete;
    Partition& operator=(const Partition&) = delete;

    MyString getLabel() const;
    size_t getAllocatedCapacity() const;
    size_t getUsedSpace() const;
    size_t getRemainingSpace() const;
    MyString getFormatStr() const;
    Directory* getRootDirectory() const;
    bool isMountedSomewhere() const;
    void setMounted(bool val);
    bool hasSpaceFor(size_t bytes) const;

    friend std::ostream& operator<<(std::ostream& os, const Partition& p);
};

#endif
