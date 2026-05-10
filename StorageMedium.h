// StorageMedium.h — Physical storage device (HDD/SSD/Optical)
// Ported from Assignment 3 — no STL used
// Faraz Aamir   | 25I-2069 | OOP Project | Spring 2026
// Shawwal       | 25I-2182 |
// Farhan        | 25I-2073 |

#ifndef STORAGE_MEDIUM_H
#define STORAGE_MEDIUM_H

#include "MyString.h"
#include "DynamicArray.h"
#include "Partition.h"
#include <iostream>

enum class MediumType { HDD, SSD, OPTICAL };

class StorageMedium {
private:
    size_t totalCapacity;
    MediumType type;
    bool mounted;
    DynamicArray<Partition*> partitions; // Composition: medium owns partitions

public:
    StorageMedium(size_t totalCapacity, MediumType type);
    ~StorageMedium();
    StorageMedium(const StorageMedium&) = delete;
    StorageMedium& operator=(const StorageMedium&) = delete;

    Partition* addPartition(const MyString& label, size_t capacity,
                            FileSystemFormat format, User* rootOwner);
    size_t getUsedCapacity() const;
    size_t getRemainingCapacity() const;
    size_t getTotalCapacity() const;
    MyString getTypeStr() const;
    bool isMounted() const;
    void setMounted(bool val);
    const DynamicArray<Partition*>& getPartitions() const;

    friend std::ostream& operator<<(std::ostream& os, const StorageMedium& sm);
};

#endif
