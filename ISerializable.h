// ISerializable.h — Abstract interface for VFS persistence
// Shared persistence contract across all domain objects
// Faraz Aamir   | 25I-2069 | OOP Project | Spring 2026
// Shawwal       | 25I-2182 |
// Farhan        | 25I-2073 |

#ifndef ISERIALIZABLE_H
#define ISERIALIZABLE_H

#include "MyString.h"

// ABSTRACTION: Any domain object that needs to be saved/loaded from VFS
// must implement this interface. The controller layer works with ISerializable*
// pointers without knowing the concrete type (polymorphism).
class ISerializable {
public:
    virtual ~ISerializable() {}

    // Convert object state to a string for VFS storage
    virtual MyString serialize() const = 0;

    // Unique identifier for storage path construction
    virtual MyString getSerializationId() const = 0;
};

#endif
