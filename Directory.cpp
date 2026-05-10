// Directory.cpp — Directory implementation
// Faraz Aamir   | 25I-2069 | OOP Project | Spring 2026
// Shawwal       | 25I-2182 |
// Farhan        | 25I-2073 |

#include "Directory.h"
#include "SymbolicLink.h"

Directory::Directory(const MyString& name, User* owner, const Permission& perms)
    : FileSystemEntity(name, owner, perms) {}

Directory::~Directory() {
    // Composition: directory owns its children — delete them all recursively
    for (int i = 0; i < children.size(); i++) {
        delete children[i];
    }
    children.clear();
}

size_t Directory::getSize() const {
    size_t total = 0;
    for (int i = 0; i < children.size(); i++) {
        total += children[i]->getSize();
    }
    return total;
}

MyString Directory::getType() const {
    return MyString("Directory");
}

FileSystemEntity* Directory::clone(User* newOwner) const {
    Directory* copy = new Directory(name, newOwner, permissions);
    // Deep copy: clone all children recursively
    for (int i = 0; i < children.size(); i++) {
        FileSystemEntity* childCopy = children[i]->clone(newOwner);
        copy->addChild(childCopy);
    }
    for (int i = 0; i < extendedAttributes.size(); i++) {
        copy->addAttribute(extendedAttributes[i].getKey(), extendedAttributes[i].getValue());
    }
    return copy;
}

// --- Child Management ---

bool Directory::addChild(FileSystemEntity* child) {
    if (!child) return false;

    // Check for duplicate name at the same level
    for (int i = 0; i < children.size(); i++) {
        if (children[i]->getName() == child->getName()) {
            std::cout << "  ERROR: Entity '" << child->getName()
                      << "' already exists in directory '" << name << "'." << std::endl;
            return false;
        }
    }

    child->setParent(this);
    children.push_back(child);
    updateModifiedTime();
    return true;
}

bool Directory::removeChild(const MyString& childName, bool deleteIt) {
    for (int i = 0; i < children.size(); i++) {
        if (children[i]->getName() == childName) {
            if (deleteIt) {
                delete children[i];
            }
            children.erase(i);
            updateModifiedTime();
            return true;
        }
    }
    std::cout << "  ERROR: Entity '" << childName << "' not found in '" << name << "'." << std::endl;
    return false;
}

FileSystemEntity* Directory::detachChild(const MyString& childName) {
    for (int i = 0; i < children.size(); i++) {
        if (children[i]->getName() == childName) {
            FileSystemEntity* child = children[i];
            children.erase(i);
            child->setParent(nullptr);
            updateModifiedTime();
            return child;
        }
    }
    return nullptr;
}

FileSystemEntity* Directory::findChild(const MyString& childName) const {
    for (int i = 0; i < children.size(); i++) {
        if (children[i]->getName() == childName) return children[i];
    }
    return nullptr;
}

const DynamicArray<FileSystemEntity*>& Directory::getChildren() const {
    return children;
}

int Directory::getChildCount() const {
    return children.size();
}

// --- Recursive Operations ---

DynamicArray<FileSystemEntity*> Directory::search(const MyString& pattern) const {
    DynamicArray<FileSystemEntity*> results;

    for (int i = 0; i < children.size(); i++) {
        // Check if name contains the pattern
        if (children[i]->getName().contains(pattern)) {
            results.push_back(children[i]);
        }
        // If it's a directory, recurse into it
        if (children[i]->getType() == "Directory" || children[i]->getType() == "MountPoint") {
            Directory* dir = dynamic_cast<Directory*>(children[i]);
            if (dir) {
                DynamicArray<FileSystemEntity*> subResults = dir->search(pattern);
                for (int j = 0; j < subResults.size(); j++) {
                    results.push_back(subResults[j]);
                }
            }
        }
    }
    return results;
}

void Directory::listShallow(std::ostream& os) const {
    os << "Contents of /" << name << "/ (" << children.size() << " items):" << std::endl;
    os << "----------------------------------------------" << std::endl;
    for (int i = 0; i < children.size(); i++) {
        children[i]->display(os, 1);
    }
    os << "----------------------------------------------" << std::endl;
    os << "Total size: " << getSize() << " bytes" << std::endl;
}

void Directory::listDeep(std::ostream& os, int indent) const {
    for (int i = 0; i < indent * 2; i++) os << ' ';
    os << permissions << " " << owner->getUsername()
       << " " << getSize() << "\t" << name << "/" << std::endl;

    for (int i = 0; i < children.size(); i++) {
        if (children[i]->getType() == "Directory" || children[i]->getType() == "MountPoint") {
            dynamic_cast<const Directory*>(children[i])->listDeep(os, indent + 1);
        } else {
            children[i]->display(os, indent + 1);
        }
    }
}

DynamicArray<FileSystemEntity*> Directory::findDanglingLinks() const {
    DynamicArray<FileSystemEntity*> dangling;
    for (int i = 0; i < children.size(); i++) {
        if (children[i]->getType() == "SymLink") {
            SymbolicLink* link = dynamic_cast<SymbolicLink*>(children[i]);
            if (link && link->isDangling()) {
                dangling.push_back(children[i]);
            }
        }
        if (children[i]->getType() == "Directory" || children[i]->getType() == "MountPoint") {
            Directory* dir = dynamic_cast<Directory*>(children[i]);
            if (dir) {
                DynamicArray<FileSystemEntity*> subDangling = dir->findDanglingLinks();
                for (int j = 0; j < subDangling.size(); j++) {
                    dangling.push_back(subDangling[j]);
                }
            }
        }
    }
    return dangling;
}

// --- Path-based operations (new for Online Judge) ---

FileSystemEntity* Directory::findByPath(const MyString& path) const {
    if (path.empty()) return nullptr;

    // Split path by '/'
    MyString parts[32];
    MyString cleanPath = path;
    // Remove leading slash if present
    if (cleanPath.length() > 0 && cleanPath[0] == '/') {
        cleanPath = cleanPath.substr(1);
    }
    int count = cleanPath.split('/', parts, 32);
    if (count == 0) return nullptr;

    const Directory* current = this;
    for (int i = 0; i < count; i++) {
        if (parts[i].empty()) continue;
        FileSystemEntity* child = current->findChild(parts[i]);
        if (!child) return nullptr;

        if (i == count - 1) return child; // Last component — return whatever we found

        // Not the last component — must be a directory to continue traversal
        if (child->getType() == "Directory" || child->getType() == "MountPoint") {
            current = dynamic_cast<Directory*>(child);
        } else {
            return nullptr; // Can't traverse into a file
        }
    }
    return nullptr;
}

Directory* Directory::findDirByPath(const MyString& path) const {
    FileSystemEntity* entity = findByPath(path);
    if (entity && (entity->getType() == "Directory" || entity->getType() == "MountPoint")) {
        return dynamic_cast<Directory*>(entity);
    }
    return nullptr;
}

Directory* Directory::createPath(const MyString& path, User* owner) {
    MyString parts[32];
    MyString cleanPath = path;
    if (cleanPath.length() > 0 && cleanPath[0] == '/') {
        cleanPath = cleanPath.substr(1);
    }
    int count = cleanPath.split('/', parts, 32);
    if (count == 0) return this;

    Directory* current = this;
    for (int i = 0; i < count; i++) {
        if (parts[i].empty()) continue;
        FileSystemEntity* child = current->findChild(parts[i]);
        if (child) {
            if (child->getType() == "Directory" || child->getType() == "MountPoint") {
                current = dynamic_cast<Directory*>(child);
            } else {
                return nullptr; // Path component exists but is not a directory
            }
        } else {
            // Create new directory
            Directory* newDir = new Directory(parts[i], owner,
                Permission(true, true, true, true, false, true, true, false, true));
            current->addChild(newDir);
            current = newDir;
        }
    }
    return current;
}

void Directory::display(std::ostream& os, int indent) const {
    for (int i = 0; i < indent * 2; i++) os << ' ';
    os << permissions << " " << owner->getUsername()
       << " " << getSize() << "\t" << name << "/" << std::endl;
}
