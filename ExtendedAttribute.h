// ExtendedAttribute.h — Key-value metadata for file system entities
// Ported from Assignment 3 — no STL used
// Faraz Aamir   | 25I-2069 | OOP Project | Spring 2026
// Shawwal       | 25I-2182 |
// Farhan        | 25I-2073 |

#ifndef EXTENDED_ATTRIBUTE_H
#define EXTENDED_ATTRIBUTE_H

#include "MyString.h"
#include <iostream>

class ExtendedAttribute {
private:
    MyString key;
    MyString value;

public:
    ExtendedAttribute();
    ExtendedAttribute(const MyString& key, const MyString& value);

    MyString getKey() const;
    MyString getValue() const;
    void setValue(const MyString& newValue);

    friend std::ostream& operator<<(std::ostream& os, const ExtendedAttribute& attr);
};

#endif
