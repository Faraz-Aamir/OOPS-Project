// Volume.cpp — Top-level system container implementation
// Faraz Aamir   | 25I-2069 | OOP Project | Spring 2026
// Shawwal       | 25I-2182 |
// Farhan        | 25I-2073 |

#include "Volume.h"

Volume::Volume(const MyString& name)
    : name(name), primaryPartition(nullptr) {}

Volume::~Volume() {
    for (int i = 0; i < media.size(); i++) delete media[i];
    media.clear();
    for (int i = 0; i < users.size(); i++) delete users[i];
    users.clear();
}

// ============================================================
// HELPERS
// ============================================================

Partition* Volume::findPartitionForEntity(FileSystemEntity* entity) const {
    if (!entity) return nullptr;
    FileSystemEntity* current = entity;
    while (current->getParent() != nullptr) {
        current = current->getParent();
    }
    for (int i = 0; i < media.size(); i++) {
        const DynamicArray<Partition*>& parts = media[i]->getPartitions();
        for (int j = 0; j < parts.size(); j++) {
            if (parts[j]->getRootDirectory() == current) return parts[j];
        }
    }
    return nullptr;
}

void Volume::invalidateSymLinks(Directory* searchDir, FileSystemEntity* target) const {
    if (!searchDir) return;
    const DynamicArray<FileSystemEntity*>& ch = searchDir->getChildren();
    for (int i = 0; i < ch.size(); i++) {
        if (ch[i]->getType() == "SymLink") {
            SymbolicLink* link = dynamic_cast<SymbolicLink*>(ch[i]);
            if (link && link->getTarget() == target) link->setTarget(nullptr);
        }
        if (ch[i]->getType() == "Directory" || ch[i]->getType() == "MountPoint") {
            invalidateSymLinks(dynamic_cast<Directory*>(ch[i]), target);
        }
    }
}

// ============================================================
// STORAGE
// ============================================================

StorageMedium* Volume::addStorageMedium(size_t capacity, MediumType type) {
    StorageMedium* medium = new StorageMedium(capacity, type);
    media.push_back(medium);
    return medium;
}

void Volume::setPrimaryPartition(Partition* partition) { primaryPartition = partition; }
Partition* Volume::getPrimaryPartition() const { return primaryPartition; }

Directory* Volume::getRoot() const {
    return primaryPartition ? primaryPartition->getRootDirectory() : nullptr;
}

// ============================================================
// USER MANAGEMENT
// ============================================================

User* Volume::addUser(const MyString& username, int userId, const MyString& homeDir,
                      UserRole role, const MyString& groupName) {
    for (int i = 0; i < users.size(); i++) {
        if (users[i]->getUsername() == username) {
            std::cout << "  ERROR: User '" << username << "' already exists." << std::endl;
            return nullptr;
        }
    }
    User* user = new User(username, userId, homeDir, role, groupName);
    users.push_back(user);
    return user;
}

User* Volume::addOnlineJudgeUser(const MyString& username, int userId,
                                  const MyString& email, const MyString& password,
                                  UserRole role) {
    // Check duplicate username
    for (int i = 0; i < users.size(); i++) {
        if (users[i]->getUsername() == username) return nullptr;
    }
    // Check duplicate email
    for (int i = 0; i < users.size(); i++) {
        if (users[i]->getEmail() == email) return nullptr;
    }

    MyString homeDir = MyString("/users/") + username;
    User* user = new User(username, userId, email, password, homeDir, role);
    users.push_back(user);

    // Create user's home directory structure in VFS
    Directory* root = getRoot();
    if (root) {
        Directory* usersDir = root->findDirByPath("users");
        if (!usersDir) {
            usersDir = root->createPath("users", user);
        }
        Directory* homeDirectory = usersDir->createPath(username, user);
        if (homeDirectory) {
            homeDirectory->createPath("solutions", user);
            homeDirectory->createPath("contests", user);
        }
    }
    return user;
}

User* Volume::findUser(const MyString& username) const {
    for (int i = 0; i < users.size(); i++) {
        if (users[i]->getUsername() == username) return users[i];
    }
    return nullptr;
}

User* Volume::findUserByEmail(const MyString& email) const {
    for (int i = 0; i < users.size(); i++) {
        if (users[i]->getEmail() == email) return users[i];
    }
    return nullptr;
}

bool Volume::removeUser(const MyString& username) {
    for (int i = 0; i < users.size(); i++) {
        if (users[i]->getUsername() == username) {
            // Delete user's VFS space recursively
            Directory* root = getRoot();
            if (root) {
                Directory* usersDir = root->findDirByPath("users");
                if (usersDir) {
                    usersDir->removeChild(username, true);
                }
            }
            delete users[i];
            users.erase(i);
            std::cout << "  User '" << username << "' and all their data deleted." << std::endl;
            return true;
        }
    }
    return false;
}

const DynamicArray<User*>& Volume::getUsers() const { return users; }

int Volume::getNextUserId() const {
    int maxId = 0;
    for (int i = 0; i < users.size(); i++) {
        if (users[i]->getUserId() > maxId) maxId = users[i]->getUserId();
    }
    return maxId + 1;
}

// ============================================================
// FILE SYSTEM OPERATIONS
// ============================================================

RegularFile* Volume::createFile(Directory* parent, const MyString& fileName,
                                const MyString& content, User* requestingUser,
                                const Permission& perms) {
    if (!parent || !requestingUser) return nullptr;
    if (!parent->checkPermission(requestingUser, "write")) return nullptr;
    Partition* partition = findPartitionForEntity(parent);
    if (partition && !partition->hasSpaceFor(content.size())) return nullptr;
    RegularFile* file = new RegularFile(fileName, requestingUser, content, perms);
    if (!parent->addChild(file)) { delete file; return nullptr; }
    return file;
}

Directory* Volume::createDirectory(Directory* parent, const MyString& dirName,
                                   User* requestingUser, const Permission& perms) {
    if (!parent || !requestingUser) return nullptr;
    if (!parent->checkPermission(requestingUser, "write")) return nullptr;
    Directory* dir = new Directory(dirName, requestingUser, perms);
    if (!parent->addChild(dir)) { delete dir; return nullptr; }
    return dir;
}

SymbolicLink* Volume::createSymLink(Directory* parent, const MyString& linkName,
                                    FileSystemEntity* target, User* requestingUser,
                                    const Permission& perms) {
    if (!parent || !requestingUser) return nullptr;
    if (!parent->checkPermission(requestingUser, "write")) return nullptr;
    SymbolicLink* link = new SymbolicLink(linkName, requestingUser, target, perms);
    if (!parent->addChild(link)) { delete link; return nullptr; }
    return link;
}

MountPoint* Volume::createMountPoint(Directory* parent, const MyString& mpName,
                                     User* requestingUser, const Permission& perms) {
    if (!parent || !requestingUser) return nullptr;
    if (!requestingUser->isSuperUser()) return nullptr;
    if (!parent->checkPermission(requestingUser, "write")) return nullptr;
    MountPoint* mp = new MountPoint(mpName, requestingUser, perms);
    if (!parent->addChild(mp)) { delete mp; return nullptr; }
    return mp;
}

bool Volume::deleteEntity(Directory* parent, const MyString& entityName,
                          User* requestingUser) {
    if (!parent || !requestingUser) return false;
    if (!parent->checkPermission(requestingUser, "write")) return false;
    FileSystemEntity* entity = parent->findChild(entityName);
    if (!entity) return false;
    if (!entity->checkPermission(requestingUser, "write")) return false;
    for (int i = 0; i < media.size(); i++) {
        const DynamicArray<Partition*>& parts = media[i]->getPartitions();
        for (int j = 0; j < parts.size(); j++) {
            invalidateSymLinks(parts[j]->getRootDirectory(), entity);
        }
    }
    parent->removeChild(entityName, true);
    return true;
}

bool Volume::modifyFile(RegularFile* file, const MyString& newContent,
                        User* requestingUser) {
    if (!file || !requestingUser) return false;
    if (!file->checkPermission(requestingUser, "write")) return false;
    if ((int)newContent.size() > (int)file->getSize()) {
        size_t extra = newContent.size() - file->getSize();
        Partition* partition = findPartitionForEntity(file);
        if (partition && !partition->hasSpaceFor(extra)) return false;
    }
    file->modifyContent(newContent, requestingUser);
    return true;
}

MyString Volume::readFile(RegularFile* file, User* requestingUser) const {
    if (!file || !requestingUser) return MyString("");
    if (!file->checkPermission(requestingUser, "read")) return MyString("");
    return file->getContent();
}

bool Volume::moveEntity(Directory* sourceDir, const MyString& entityName,
                        Directory* destDir, User* requestingUser) {
    if (!sourceDir || !destDir || !requestingUser) return false;
    if (!sourceDir->checkPermission(requestingUser, "write")) return false;
    if (!destDir->checkPermission(requestingUser, "write")) return false;
    FileSystemEntity* entity = sourceDir->detachChild(entityName);
    if (!entity) return false;
    if (!destDir->addChild(entity)) {
        sourceDir->addChild(entity);
        return false;
    }
    return true;
}

FileSystemEntity* Volume::copyEntity(FileSystemEntity* entity, Directory* destDir,
                                     User* requestingUser) {
    if (!entity || !destDir || !requestingUser) return nullptr;
    if (!entity->checkPermission(requestingUser, "read")) return nullptr;
    if (!destDir->checkPermission(requestingUser, "write")) return nullptr;
    Partition* destPartition = findPartitionForEntity(destDir);
    if (destPartition && !destPartition->hasSpaceFor(entity->getSize())) return nullptr;
    FileSystemEntity* copy = entity->clone(requestingUser);
    if (!destDir->addChild(copy)) { delete copy; return nullptr; }
    return copy;
}

DynamicArray<FileSystemEntity*> Volume::search(Directory* startDir, const MyString& pattern) const {
    if (!startDir) return DynamicArray<FileSystemEntity*>();
    return startDir->search(pattern);
}

size_t Volume::computeSubtreeSize(Directory* dir) const {
    return dir ? dir->getSize() : 0;
}

void Volume::listShallow(Directory* dir, User* requestingUser) const {
    if (!dir || !requestingUser) return;
    if (!dir->checkPermission(requestingUser, "read")) return;
    dir->listShallow(std::cout);
}

void Volume::listDeep(Directory* dir, User* requestingUser) const {
    if (!dir || !requestingUser) return;
    if (!dir->checkPermission(requestingUser, "read")) return;
    dir->listDeep(std::cout, 0);
}

DynamicArray<FileSystemEntity*> Volume::detectDanglingLinks() const {
    DynamicArray<FileSystemEntity*> allDangling;
    for (int i = 0; i < media.size(); i++) {
        const DynamicArray<Partition*>& parts = media[i]->getPartitions();
        for (int j = 0; j < parts.size(); j++) {
            DynamicArray<FileSystemEntity*> d = parts[j]->getRootDirectory()->findDanglingLinks();
            for (int k = 0; k < d.size(); k++) allDangling.push_back(d[k]);
        }
    }
    return allDangling;
}

bool Volume::changePermissions(FileSystemEntity* entity, User* requestingUser,
                               const MyString& relation, const MyString& perm, bool value) {
    if (!entity || !requestingUser) return false;
    if (!requestingUser->isSuperUser() &&
        requestingUser->getUserId() != entity->getOwner()->getUserId()) return false;
    if (perm == "read") entity->getPermissions().setRead(relation, value);
    else if (perm == "write") entity->getPermissions().setWrite(relation, value);
    else if (perm == "execute") entity->getPermissions().setExecute(relation, value);
    return true;
}

bool Volume::restoreVersion(RegularFile* file, int versionIndex, User* requestingUser) {
    if (!file || !requestingUser) return false;
    if (!file->checkPermission(requestingUser, "write")) return false;
    file->restoreVersion(versionIndex, requestingUser);
    return true;
}

// ============================================================
// QUICK VFS HELPERS FOR ONLINE JUDGE
// ============================================================

RegularFile* Volume::writeToVFS(const MyString& path, const MyString& content, User* user) {
    // Parse path into directory and filename
    int lastSlash = -1;
    for (int i = 0; i < path.length(); i++) {
        if (path[i] == '/') lastSlash = i;
    }

    MyString dirPath, fileName;
    if (lastSlash == -1) {
        dirPath = "";
        fileName = path;
    } else {
        dirPath = path.substr(0, lastSlash);
        fileName = path.substr(lastSlash + 1);
    }

    Directory* root = getRoot();
    if (!root) return nullptr;

    // Ensure directory exists
    Directory* dir = root;
    if (!dirPath.empty()) {
        dir = root->createPath(dirPath, user);
        if (!dir) return nullptr;
    }

    // Check if file already exists — update it
    FileSystemEntity* existing = dir->findChild(fileName);
    if (existing && existing->getType() == "File") {
        RegularFile* file = dynamic_cast<RegularFile*>(existing);
        file->setContent(content);
        return file;
    }

    // Create new file
    return createFile(dir, fileName, content, user);
}

MyString Volume::readFromVFS(const MyString& path, User* user) const {
    Directory* root = getRoot();
    if (!root) return MyString("");

    FileSystemEntity* entity = root->findByPath(path);
    if (!entity || entity->getType() != "File") return MyString("");

    RegularFile* file = dynamic_cast<RegularFile*>(entity);
    return file->getContent();
}

Directory* Volume::ensureDir(const MyString& path, User* user) {
    Directory* root = getRoot();
    if (!root) return nullptr;
    return root->createPath(path, user);
}

bool Volume::deleteFromVFS(const MyString& path, User* user) {
    Directory* root = getRoot();
    if (!root) return false;

    int lastSlash = -1;
    for (int i = 0; i < path.length(); i++) {
        if (path[i] == '/') lastSlash = i;
    }

    if (lastSlash == -1) {
        return deleteEntity(root, path, user);
    }

    MyString dirPath = path.substr(0, lastSlash);
    MyString entityName = path.substr(lastSlash + 1);
    Directory* dir = root->findDirByPath(dirPath);
    if (!dir) return false;
    return deleteEntity(dir, entityName, user);
}

bool Volume::existsInVFS(const MyString& path) const {
    Directory* root = getRoot();
    if (!root) return false;
    return root->findByPath(path) != nullptr;
}

// ============================================================
// DISPLAY
// ============================================================

void Volume::printFinalState(std::ostream& os) const {
    os << "\n============ VOLUME: " << name << " ============" << std::endl;
    os << "Users: " << users.size() << std::endl;
    for (int i = 0; i < users.size(); i++) {
        os << "  " << *users[i] << std::endl;
    }
    os << "\nStorage: " << media.size() << " medium(s)" << std::endl;
    for (int i = 0; i < media.size(); i++) {
        os << "  " << *media[i] << std::endl;
    }
    if (primaryPartition) {
        os << "\nDirectory Tree:" << std::endl;
        primaryPartition->getRootDirectory()->listDeep(os, 0);
    }
    os << "=========================================\n" << std::endl;
}

std::ostream& operator<<(std::ostream& os, const Volume& v) {
    os << "Volume[" << v.name << ", media=" << v.media.size()
       << ", users=" << v.users.size() << "]";
    return os;
}
