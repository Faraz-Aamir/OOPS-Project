// VersionRecord.cpp — File version history record implementation
// Faraz Aamir   | 25I-2069 | OOP Project | Spring 2026
// Shawwal       | 25I-2182 |
// Farhan        | 25I-2073 |

#include "VersionRecord.h"

VersionRecord::VersionRecord() : content(""), timestamp(0), modifiedBy(nullptr) {}

VersionRecord::VersionRecord(const MyString& content, User* modifiedBy)
    : content(content), modifiedBy(modifiedBy) {
    timestamp = time(nullptr);
}

MyString VersionRecord::getContent() const { return content; }
time_t VersionRecord::getTimestamp() const { return timestamp; }
User* VersionRecord::getModifiedBy() const { return modifiedBy; }

MyString VersionRecord::getTimestampStr() const {
    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&timestamp));
    return MyString(buf);
}

std::ostream& operator<<(std::ostream& os, const VersionRecord& vr) {
    os << "Version[by=" << (vr.modifiedBy ? vr.modifiedBy->getUsername() : MyString("unknown"))
       << ", time=" << vr.getTimestampStr()
       << ", size=" << vr.content.size() << " bytes]";
    return os;
}
