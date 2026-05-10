// MountPoint.h — Special directory where a partition is mounted
// Ported from Assignment 3 — no STL used
// Faraz Aamir   | 25I-2069 | OOP Project | Spring 2026
// Shawwal       | 25I-2182 |
// Farhan        | 25I-2073 |

#ifndef MOUNT_POINT_H
#define MOUNT_POINT_H

#include "Directory.h"

class Partition; // Forward declaration

class MountPoint : public Directory {
private:
    Partition* mountedPartition; // Aggregation: does NOT own the partition

public:
    MountPoint(const MyString& name, User* owner,
               const Permission& perms = Permission());
    ~MountPoint() override;

    MyString getType() const override;

    bool mount(Partition* partition);
    bool unmount();
    bool isMounted() const;
    Partition* getMountedPartition() const;

    void display(std::ostream& os, int indent = 0) const override;
};

#endif
