// Directory.h — Directory entity (owns children via composition)
// Ported from Assignment 3 — no STL used
// Faraz Aamir   | 25I-2069 | OOP Project | Spring 2026
// Shawwal       | 25I-2182 |
// Farhan        | 25I-2073 |

#ifndef DIRECTORY_H
#define DIRECTORY_H

#include "FileSystemEntity.h"
#include "DynamicArray.h"

class Directory : public FileSystemEntity {
protected:
    DynamicArray<FileSystemEntity*> children; // Composition: directory owns children

public:
    Directory(const MyString& name, User* owner,
              const Permission& perms = Permission());
    ~Directory() override; // Recursively deletes all children

    // Override pure virtuals
    size_t getSize() const override;
    MyString getType() const override;
    FileSystemEntity* clone(User* newOwner) const override;

    // Child management
    bool addChild(FileSystemEntity* child);
    bool removeChild(const MyString& childName, bool deleteIt = true);
    FileSystemEntity* detachChild(const MyString& childName);
    FileSystemEntity* findChild(const MyString& childName) const;
    const DynamicArray<FileSystemEntity*>& getChildren() const;
    int getChildCount() const;

    // Recursive operations
    DynamicArray<FileSystemEntity*> search(const MyString& pattern) const;
    void listShallow(std::ostream& os) const;
    void listDeep(std::ostream& os, int indent = 0) const;

    // Dangling symlink detection (recursive)
    DynamicArray<FileSystemEntity*> findDanglingLinks() const;

    // Find entity by path (e.g., "users/fatima/solutions")
    FileSystemEntity* findByPath(const MyString& path) const;
    Directory* findDirByPath(const MyString& path) const;

    // Create nested directory structure (mkdir -p equivalent)
    Directory* createPath(const MyString& path, User* owner);

    void display(std::ostream& os, int indent = 0) const override;
};

#endif
