// Partition.cpp — Partition implementation
// Faraz Aamir   | 25I-2069 | OOP Project | Spring 2026
// Shawwal       | 25I-2182 |
// Farhan        | 25I-2073 |

#include "Partition.h"

Partition::Partition(const MyString& label, size_t capacity,
                     FileSystemFormat format, User* rootOwner)
    : label(label), allocatedCapacity(capacity), formatType(format),
      mounted(false) {
    rootDirectory = new Directory("root", rootOwner,
                                 Permission(true, true, true, true, false, true, true, false, true));
}

Partition::~Partition() {
    delete rootDirectory; // Composition: destroy root and all children recursively
}

MyString Partition::getLabel() const { return label; }
size_t Partition::getAllocatedCapacity() const { return allocatedCapacity; }

size_t Partition::getUsedSpace() const {
    return rootDirectory ? rootDirectory->getSize() : 0;
}

size_t Partition::getRemainingSpace() const {
    size_t used = getUsedSpace();
    if (used >= allocatedCapacity) return 0;
    return allocatedCapacity - used;
}

MyString Partition::getFormatStr() const {
    switch (formatType) {
        case FileSystemFormat::FAT32: return MyString("FAT32");
        case FileSystemFormat::NTFS:  return MyString("NTFS");
        case FileSystemFormat::EXT4:  return MyString("ext4");
    }
    return MyString("Unknown");
}

Directory* Partition::getRootDirectory() const { return rootDirectory; }
bool Partition::isMountedSomewhere() const { return mounted; }
void Partition::setMounted(bool val) { mounted = val; }

bool Partition::hasSpaceFor(size_t bytes) const {
    return getRemainingSpace() >= bytes;
}

std::ostream& operator<<(std::ostream& os, const Partition& p) {
    os << "Partition[" << p.label << ", " << p.getFormatStr()
       << ", capacity=" << p.allocatedCapacity
       << ", used=" << p.getUsedSpace()
       << ", free=" << p.getRemainingSpace()
       << ", mounted=" << (p.mounted ? "yes" : "no") << "]";
    return os;
}
