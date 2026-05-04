// Permission.cpp — File/directory permission model implementation
// Ported from Assignment 3 — no STL used
// Faraz Aamir | 25I-2069 | OOP Project | Spring 2026

#include "Permission.h"

Permission::Permission(bool oR, bool oW, bool oX,
                       bool gR, bool gW, bool gX,
                       bool tR, bool tW, bool tX)
    : ownerRead(oR), ownerWrite(oW), ownerExecute(oX),
      groupRead(gR), groupWrite(gW), groupExecute(gX),
      otherRead(tR), otherWrite(tW), otherExecute(tX) {}

bool Permission::canRead(const MyString& relation) const {
    if (relation == "owner") return ownerRead;
    if (relation == "group") return groupRead;
    return otherRead;
}

bool Permission::canWrite(const MyString& relation) const {
    if (relation == "owner") return ownerWrite;
    if (relation == "group") return groupWrite;
    return otherWrite;
}

bool Permission::canExecute(const MyString& relation) const {
    if (relation == "owner") return ownerExecute;
    if (relation == "group") return groupExecute;
    return otherExecute;
}

void Permission::setRead(const MyString& relation, bool value) {
    if (relation == "owner") ownerRead = value;
    else if (relation == "group") groupRead = value;
    else otherRead = value;
}

void Permission::setWrite(const MyString& relation, bool value) {
    if (relation == "owner") ownerWrite = value;
    else if (relation == "group") groupWrite = value;
    else otherWrite = value;
}

void Permission::setExecute(const MyString& relation, bool value) {
    if (relation == "owner") ownerExecute = value;
    else if (relation == "group") groupExecute = value;
    else otherExecute = value;
}

MyString Permission::toString() const {
    char result[10];
    result[0] = ownerRead ? 'r' : '-';
    result[1] = ownerWrite ? 'w' : '-';
    result[2] = ownerExecute ? 'x' : '-';
    result[3] = groupRead ? 'r' : '-';
    result[4] = groupWrite ? 'w' : '-';
    result[5] = groupExecute ? 'x' : '-';
    result[6] = otherRead ? 'r' : '-';
    result[7] = otherWrite ? 'w' : '-';
    result[8] = otherExecute ? 'x' : '-';
    result[9] = '\0';
    return MyString(result);
}

std::ostream& operator<<(std::ostream& os, const Permission& p) {
    os << p.toString();
    return os;
}
