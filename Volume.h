// Volume.h — Top-level system container
// Ported from Assignment 3 + extended for Online Judge
// Includes disk persistence for VFS state across shutdown/restart
// Faraz Aamir   | 25I-2069 | OOP Project | Spring 2026
// Shawwal       | 25I-2182 |
// Farhan        | 25I-2073 |

#ifndef VOLUME_H
#define VOLUME_H

#include "MyString.h"
#include "DynamicArray.h"
#include "StorageMedium.h"
#include "MountPoint.h"
#include "RegularFile.h"
#include "SymbolicLink.h"
#include "User.h"

class Volume {
private:
    MyString name;
    DynamicArray<StorageMedium*> media;   // Composition
    DynamicArray<User*> users;            // Composition
    Partition* primaryPartition;

    Partition* findPartitionForEntity(FileSystemEntity* entity) const;
    void invalidateSymLinks(Directory* searchDir, FileSystemEntity* target) const;

public:
    Volume(const MyString& name);
    ~Volume();
    Volume(const Volume&) = delete;
    Volume& operator=(const Volume&) = delete;

    // --- Storage ---
    StorageMedium* addStorageMedium(size_t capacity, MediumType type);
    void setPrimaryPartition(Partition* partition);
    Partition* getPrimaryPartition() const;
    Directory* getRoot() const; // Shortcut to primary partition root

    // --- User Management ---
    User* addUser(const MyString& username, int userId, const MyString& homeDir,
                  UserRole role, const MyString& groupName = "users");
    User* addOnlineJudgeUser(const MyString& username, int userId, const MyString& email,
                             const MyString& password, UserRole role);
    User* findUser(const MyString& username) const;
    User* findUserByEmail(const MyString& email) const;
    bool removeUser(const MyString& username);
    const DynamicArray<User*>& getUsers() const;
    int getNextUserId() const;

    // --- File System Operations ---
    RegularFile* createFile(Directory* parent, const MyString& fileName,
                           const MyString& content, User* requestingUser,
                           const Permission& perms = Permission());
    Directory* createDirectory(Directory* parent, const MyString& dirName,
                              User* requestingUser,
                              const Permission& perms = Permission());
    SymbolicLink* createSymLink(Directory* parent, const MyString& linkName,
                               FileSystemEntity* target, User* requestingUser,
                               const Permission& perms = Permission());
    MountPoint* createMountPoint(Directory* parent, const MyString& mpName,
                                User* requestingUser,
                                const Permission& perms = Permission());
    bool deleteEntity(Directory* parent, const MyString& entityName,
                     User* requestingUser);
    bool modifyFile(RegularFile* file, const MyString& newContent,
                   User* requestingUser);
    MyString readFile(RegularFile* file, User* requestingUser) const;
    bool moveEntity(Directory* sourceDir, const MyString& entityName,
                   Directory* destDir, User* requestingUser);
    FileSystemEntity* copyEntity(FileSystemEntity* entity, Directory* destDir,
                                User* requestingUser);
    DynamicArray<FileSystemEntity*> search(Directory* startDir, const MyString& pattern) const;
    size_t computeSubtreeSize(Directory* dir) const;
    void listShallow(Directory* dir, User* requestingUser) const;
    void listDeep(Directory* dir, User* requestingUser) const;
    DynamicArray<FileSystemEntity*> detectDanglingLinks() const;
    bool changePermissions(FileSystemEntity* entity, User* requestingUser,
                          const MyString& relation, const MyString& perm, bool value);
    bool restoreVersion(RegularFile* file, int versionIndex, User* requestingUser);

    // --- Quick VFS helpers for Online Judge ---
    RegularFile* writeToVFS(const MyString& path, const MyString& content, User* user);
    MyString readFromVFS(const MyString& path, User* user) const;
    Directory* ensureDir(const MyString& path, User* user);
    bool deleteFromVFS(const MyString& path, User* user);
    bool existsInVFS(const MyString& path) const;

    // --- Disk Persistence (VFS state across shutdown/restart) ---
    bool saveToDisk(const char* filePath) const;
    int loadUsersFromDisk(const char* filePath);

    // --- Display ---
    void printFinalState(std::ostream& os) const;
    friend std::ostream& operator<<(std::ostream& os, const Volume& v);
};

#endif
