// FileSystemEntity.h — Abstract base class for all file system entities
// Ported from Assignment 3 — no STL used
// Faraz Aamir   | 25I-2069 | OOP Project | Spring 2026
// Shawwal       | 25I-2182 |
// Farhan        | 25I-2073 |

#ifndef FILE_SYSTEM_ENTITY_H
#define FILE_SYSTEM_ENTITY_H

#include "MyString.h"
#include "DynamicArray.h"
#include "User.h"
#include "Permission.h"
#include "ExtendedAttribute.h"
#include <ctime>
#include <iostream>

class Directory; // Forward declaration

class FileSystemEntity {
protected:
    MyString name;
    time_t createdAt;
    time_t modifiedAt;
    User* owner;                                    // Aggregation: entity does NOT own the user
    Permission permissions;                          // Composition: permissions die with entity
    DynamicArray<ExtendedAttribute> extendedAttributes; // Composition
    Directory* parent;                               // Pointer to parent directory

public:
    FileSystemEntity(const MyString& name, User* owner,
                     const Permission& perms = Permission());
    virtual ~FileSystemEntity();

    // Prevent accidental copies — use clone() for explicit deep copy
    FileSystemEntity(const FileSystemEntity&) = delete;
    FileSystemEntity& operator=(const FileSystemEntity&) = delete;

    // Pure virtual functions — must be overridden by subclasses
    virtual size_t getSize() const = 0;
    virtual MyString getType() const = 0;
    virtual FileSystemEntity* clone(User* newOwner) const = 0;

    // Getters
    MyString getName() const;
    time_t getCreatedAt() const;
    time_t getModifiedAt() const;
    User* getOwner() const;
    Permission& getPermissions();
    const Permission& getPermissions() const;
    Directory* getParent() const;
    MyString getCreatedAtStr() const;
    MyString getModifiedAtStr() const;

    // Setters
    void setName(const MyString& newName);
    void setOwner(User* newOwner);
    void setParent(Directory* dir);
    void updateModifiedTime();

    // Extended Attributes management
    void addAttribute(const MyString& key, const MyString& value);
    bool removeAttribute(const MyString& key);
    MyString getAttribute(const MyString& key) const;
    const DynamicArray<ExtendedAttribute>& getAttributes() const;

    // Permission checking
    bool checkPermission(User* user, const MyString& action) const;
    MyString getRelation(User* user) const;

    // Display
    virtual void display(std::ostream& os, int indent = 0) const;
    MyString getFullPath() const;

    friend std::ostream& operator<<(std::ostream& os, const FileSystemEntity& entity);
};

#endif
