// MountPoint.cpp — MountPoint implementation
// Faraz Aamir   | 25I-2069 | OOP Project | Spring 2026
// Shawwal       | 25I-2182 |
// Farhan        | 25I-2073 |

#include "MountPoint.h"
#include "Partition.h"

MountPoint::MountPoint(const MyString& name, User* owner, const Permission& perms)
    : Directory(name, owner, perms), mountedPartition(nullptr) {}

MountPoint::~MountPoint() {
    // Does NOT delete mounted partition (aggregation)
    // Unmount before destruction
    if (mountedPartition) {
        mountedPartition->setMounted(false);
    }
}

MyString MountPoint::getType() const { return MyString("MountPoint"); }

bool MountPoint::mount(Partition* partition) {
    if (!partition) return false;
    if (mountedPartition) {
        std::cout << "  ERROR: Mount point '" << name << "' already has a partition mounted." << std::endl;
        return false;
    }
    if (partition->isMountedSomewhere()) {
        std::cout << "  ERROR: Partition '" << partition->getLabel()
                  << "' is already mounted elsewhere." << std::endl;
        return false;
    }
    mountedPartition = partition;
    partition->setMounted(true);
    std::cout << "  Mounted partition '" << partition->getLabel()
              << "' at '" << name << "'." << std::endl;
    return true;
}

bool MountPoint::unmount() {
    if (!mountedPartition) {
        std::cout << "  ERROR: Nothing mounted at '" << name << "'." << std::endl;
        return false;
    }
    std::cout << "  Unmounted partition '" << mountedPartition->getLabel()
              << "' from '" << name << "'." << std::endl;
    mountedPartition->setMounted(false);
    mountedPartition = nullptr;
    return true;
}

bool MountPoint::isMounted() const { return mountedPartition != nullptr; }
Partition* MountPoint::getMountedPartition() const { return mountedPartition; }

void MountPoint::display(std::ostream& os, int indent) const {
    for (int i = 0; i < indent * 2; i++) os << ' ';
    os << permissions << " " << owner->getUsername()
       << " " << getSize() << "\t" << name << "/ [mount]" << std::endl;
}
