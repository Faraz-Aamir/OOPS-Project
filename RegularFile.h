// RegularFile.h — Regular file entity
// Ported from Assignment 3 — no STL used
// Faraz Aamir   | 25I-2069 | OOP Project | Spring 2026
// Shawwal       | 25I-2182 |
// Farhan        | 25I-2073 |

#ifndef REGULAR_FILE_H
#define REGULAR_FILE_H

#include "FileSystemEntity.h"
#include "VersionRecord.h"
#include "DynamicArray.h"

class RegularFile : public FileSystemEntity {
private:
    MyString content;
    DynamicArray<VersionRecord> versionHistory; // Composition: dies with file

public:
    RegularFile(const MyString& name, User* owner,
                const MyString& content = "",
                const Permission& perms = Permission());
    ~RegularFile() override;

    // Override pure virtuals
    size_t getSize() const override;
    MyString getType() const override;
    FileSystemEntity* clone(User* newOwner) const override;

    // File-specific operations
    MyString getContent() const;
    void setContent(const MyString& newContent);
    void modifyContent(const MyString& newContent, User* modifiedBy);
    void restoreVersion(int versionIndex, User* restoredBy);

    // Version history
    const DynamicArray<VersionRecord>& getVersionHistory() const;
    int getVersionCount() const;

    void display(std::ostream& os, int indent = 0) const override;
};

#endif
