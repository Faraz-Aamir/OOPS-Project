// SymbolicLink.h — Symbolic link entity
// Ported from Assignment 3 — no STL used
// Faraz Aamir | 25I-2069 | OOP Project | Spring 2026

#ifndef SYMBOLIC_LINK_H
#define SYMBOLIC_LINK_H

#include "FileSystemEntity.h"

class SymbolicLink : public FileSystemEntity {
private:
    FileSystemEntity* target; // Aggregation: link does NOT own the target

public:
    SymbolicLink(const MyString& name, User* owner, FileSystemEntity* target,
                 const Permission& perms = Permission());
    ~SymbolicLink() override;

    size_t getSize() const override;
    MyString getType() const override;
    FileSystemEntity* clone(User* newOwner) const override;

    FileSystemEntity* resolve() const;
    bool isDangling() const;
    FileSystemEntity* getTarget() const;
    void setTarget(FileSystemEntity* newTarget);

    void display(std::ostream& os, int indent = 0) const override;
};

#endif
