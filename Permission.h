// Permission.h — File/directory permission model (rwxrwxrwx)
// Ported from Assignment 3 — no STL used
// Faraz Aamir   | 25I-2069 | OOP Project | Spring 2026
// Shawwal       | 25I-2182 |
// Farhan        | 25I-2073 |

#ifndef PERMISSION_H
#define PERMISSION_H

#include "MyString.h"
#include <iostream>

class Permission {
private:
    bool ownerRead, ownerWrite, ownerExecute;
    bool groupRead, groupWrite, groupExecute;
    bool otherRead, otherWrite, otherExecute;

public:
    Permission(bool oR = true, bool oW = true, bool oX = false,
               bool gR = true, bool gW = false, bool gX = false,
               bool tR = true, bool tW = false, bool tX = false);

    // Check permissions based on relation: "owner", "group", "other"
    bool canRead(const MyString& relation) const;
    bool canWrite(const MyString& relation) const;
    bool canExecute(const MyString& relation) const;

    // Modify permissions
    void setRead(const MyString& relation, bool value);
    void setWrite(const MyString& relation, bool value);
    void setExecute(const MyString& relation, bool value);

    // Display as rwxrwxrwx format
    MyString toString() const;

    friend std::ostream& operator<<(std::ostream& os, const Permission& p);
};

#endif
