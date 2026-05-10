// FileSystemEntity.cpp — Abstract base class implementation
// Faraz Aamir   | 25I-2069 | OOP Project | Spring 2026
// Shawwal       | 25I-2182 |
// Farhan        | 25I-2073 |

#include "FileSystemEntity.h"
#include "Directory.h"

FileSystemEntity::FileSystemEntity(const MyString& name, User* owner,
                                   const Permission& perms)
    : name(name), owner(owner), permissions(perms), parent(nullptr) {
    createdAt = time(nullptr);
    modifiedAt = createdAt;
}

FileSystemEntity::~FileSystemEntity() {
    // Does NOT delete owner (aggregation)
    // extendedAttributes cleaned up by DynamicArray destructor (composition)
}

// --- Getters ---
MyString FileSystemEntity::getName() const { return name; }
time_t FileSystemEntity::getCreatedAt() const { return createdAt; }
time_t FileSystemEntity::getModifiedAt() const { return modifiedAt; }
User* FileSystemEntity::getOwner() const { return owner; }
Permission& FileSystemEntity::getPermissions() { return permissions; }
const Permission& FileSystemEntity::getPermissions() const { return permissions; }
Directory* FileSystemEntity::getParent() const { return parent; }

MyString FileSystemEntity::getCreatedAtStr() const {
    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&createdAt));
    return MyString(buf);
}

MyString FileSystemEntity::getModifiedAtStr() const {
    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&modifiedAt));
    return MyString(buf);
}

// --- Setters ---
void FileSystemEntity::setName(const MyString& newName) { name = newName; }
void FileSystemEntity::setOwner(User* newOwner) { owner = newOwner; }
void FileSystemEntity::setParent(Directory* dir) { parent = dir; }
void FileSystemEntity::updateModifiedTime() { modifiedAt = time(nullptr); }

// --- Extended Attributes ---
void FileSystemEntity::addAttribute(const MyString& key, const MyString& value) {
    // Check if key already exists — update if so
    for (int i = 0; i < extendedAttributes.size(); i++) {
        if (extendedAttributes[i].getKey() == key) {
            extendedAttributes[i].setValue(value);
            return;
        }
    }
    extendedAttributes.push_back(ExtendedAttribute(key, value));
}

bool FileSystemEntity::removeAttribute(const MyString& key) {
    for (int i = 0; i < extendedAttributes.size(); i++) {
        if (extendedAttributes[i].getKey() == key) {
            extendedAttributes.erase(i);
            return true;
        }
    }
    return false;
}

MyString FileSystemEntity::getAttribute(const MyString& key) const {
    for (int i = 0; i < extendedAttributes.size(); i++) {
        if (extendedAttributes[i].getKey() == key) {
            return extendedAttributes[i].getValue();
        }
    }
    return MyString("");
}

const DynamicArray<ExtendedAttribute>& FileSystemEntity::getAttributes() const {
    return extendedAttributes;
}

// --- Permission Checking ---
MyString FileSystemEntity::getRelation(User* user) const {
    if (!user || !owner) return MyString("other");
    if (user->isSuperUser()) return MyString("owner"); // Superuser always has owner-level
    if (user->getUserId() == owner->getUserId()) return MyString("owner");
    if (user->getGroupName() == owner->getGroupName()) return MyString("group");
    return MyString("other");
}

bool FileSystemEntity::checkPermission(User* user, const MyString& action) const {
    if (!user) return false;
    if (user->isSuperUser()) return true; // Superuser bypasses all checks

    MyString relation = getRelation(user);
    if (action == "read") return permissions.canRead(relation);
    if (action == "write") return permissions.canWrite(relation);
    if (action == "execute") return permissions.canExecute(relation);
    return false;
}

// --- Display ---
void FileSystemEntity::display(std::ostream& os, int indent) const {
    for (int i = 0; i < indent * 2; i++) os << ' ';
    os << permissions << " " << (owner ? owner->getUsername() : MyString("???"))
       << " " << getSize() << "\t" << name << std::endl;
}

MyString FileSystemEntity::getFullPath() const {
    if (!parent) return MyString("/") + name;
    return parent->getFullPath() + "/" + name;
}

std::ostream& operator<<(std::ostream& os, const FileSystemEntity& entity) {
    entity.display(os, 0);
    return os;
}
