// StorageMedium.cpp — Storage medium implementation
// Faraz Aamir | 25I-2069 | OOP Project | Spring 2026

#include "StorageMedium.h"

StorageMedium::StorageMedium(size_t totalCapacity, MediumType type)
    : totalCapacity(totalCapacity), type(type), mounted(true) {}

StorageMedium::~StorageMedium() {
    for (int i = 0; i < partitions.size(); i++) {
        delete partitions[i];
    }
    partitions.clear();
}

Partition* StorageMedium::addPartition(const MyString& label, size_t capacity,
                                       FileSystemFormat format, User* rootOwner) {
    if (capacity > getRemainingCapacity()) {
        std::cout << "  ERROR: Not enough unallocated space. Requested: "
                  << capacity << ", Available: " << getRemainingCapacity() << std::endl;
        return nullptr;
    }
    Partition* p = new Partition(label, capacity, format, rootOwner);
    partitions.push_back(p);
    return p;
}

size_t StorageMedium::getUsedCapacity() const {
    size_t total = 0;
    for (int i = 0; i < partitions.size(); i++) {
        total += partitions[i]->getAllocatedCapacity();
    }
    return total;
}

size_t StorageMedium::getRemainingCapacity() const {
    size_t used = getUsedCapacity();
    return (used >= totalCapacity) ? 0 : totalCapacity - used;
}

size_t StorageMedium::getTotalCapacity() const { return totalCapacity; }

MyString StorageMedium::getTypeStr() const {
    switch (type) {
        case MediumType::HDD:     return MyString("HDD");
        case MediumType::SSD:     return MyString("SSD");
        case MediumType::OPTICAL: return MyString("Optical");
    }
    return MyString("Unknown");
}

bool StorageMedium::isMounted() const { return mounted; }
void StorageMedium::setMounted(bool val) { mounted = val; }
const DynamicArray<Partition*>& StorageMedium::getPartitions() const { return partitions; }

std::ostream& operator<<(std::ostream& os, const StorageMedium& sm) {
    os << "StorageMedium[" << sm.getTypeStr()
       << ", total=" << sm.totalCapacity
       << ", allocated=" << sm.getUsedCapacity()
       << ", free=" << sm.getRemainingCapacity()
       << ", partitions=" << sm.partitions.size() << "]";
    return os;
}
