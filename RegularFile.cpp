// RegularFile.cpp — Regular file implementation
// Faraz Aamir   | 25I-2069 | OOP Project | Spring 2026
// Shawwal       | 25I-2182 |
// Farhan        | 25I-2073 |

#include "RegularFile.h"

RegularFile::RegularFile(const MyString& name, User* owner,
                         const MyString& content, const Permission& perms)
    : FileSystemEntity(name, owner, perms), content(content) {}

RegularFile::~RegularFile() {
    // versionHistory cleaned up by DynamicArray destructor (composition)
}

size_t RegularFile::getSize() const {
    return content.size();
}

MyString RegularFile::getType() const {
    return MyString("File");
}

FileSystemEntity* RegularFile::clone(User* newOwner) const {
    RegularFile* copy = new RegularFile(name, newOwner, content, permissions);
    for (int i = 0; i < extendedAttributes.size(); i++) {
        copy->addAttribute(extendedAttributes[i].getKey(), extendedAttributes[i].getValue());
    }
    return copy;
}

MyString RegularFile::getContent() const { return content; }

void RegularFile::setContent(const MyString& newContent) {
    content = newContent;
    updateModifiedTime();
}

void RegularFile::modifyContent(const MyString& newContent, User* modifiedBy) {
    // Save current version before modifying
    versionHistory.push_back(VersionRecord(content, modifiedBy));
    content = newContent;
    updateModifiedTime();
}

void RegularFile::restoreVersion(int versionIndex, User* restoredBy) {
    if (versionIndex < 0 || versionIndex >= versionHistory.size()) {
        std::cout << "  ERROR: Invalid version index " << versionIndex
                  << ". Available: 0 to " << versionHistory.size() - 1 << std::endl;
        return;
    }
    versionHistory.push_back(VersionRecord(content, restoredBy));
    content = versionHistory[versionIndex].getContent();
    updateModifiedTime();
    std::cout << "  Restored to version " << versionIndex << " successfully." << std::endl;
}

const DynamicArray<VersionRecord>& RegularFile::getVersionHistory() const {
    return versionHistory;
}

int RegularFile::getVersionCount() const {
    return versionHistory.size();
}

void RegularFile::display(std::ostream& os, int indent) const {
    for (int i = 0; i < indent * 2; i++) os << ' ';
    os << permissions << " " << owner->getUsername()
       << " " << getSize() << "\t" << name << std::endl;
}
