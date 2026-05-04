// ExtendedAttribute.cpp — Key-value metadata implementation
// Faraz Aamir | 25I-2069 | OOP Project | Spring 2026

#include "ExtendedAttribute.h"

ExtendedAttribute::ExtendedAttribute() : key(""), value("") {}

ExtendedAttribute::ExtendedAttribute(const MyString& key, const MyString& value)
    : key(key), value(value) {}

MyString ExtendedAttribute::getKey() const { return key; }
MyString ExtendedAttribute::getValue() const { return value; }
void ExtendedAttribute::setValue(const MyString& newValue) { value = newValue; }

std::ostream& operator<<(std::ostream& os, const ExtendedAttribute& attr) {
    os << attr.key << " = " << attr.value;
    return os;
}
