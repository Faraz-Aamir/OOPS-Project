// VersionRecord.h — File version history record
// Ported from Assignment 3 — no STL used
// Faraz Aamir   | 25I-2069 | OOP Project | Spring 2026
// Shawwal       | 25I-2182 |
// Farhan        | 25I-2073 |

#ifndef VERSION_RECORD_H
#define VERSION_RECORD_H

#include "MyString.h"
#include "User.h"
#include <ctime>
#include <iostream>

class VersionRecord {
private:
    MyString content;
    time_t timestamp;
    User* modifiedBy;

public:
    VersionRecord();
    VersionRecord(const MyString& content, User* modifiedBy);

    MyString getContent() const;
    time_t getTimestamp() const;
    User* getModifiedBy() const;
    MyString getTimestampStr() const;

    friend std::ostream& operator<<(std::ostream& os, const VersionRecord& vr);
};

#endif
