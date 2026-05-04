// SessionManager.h — Login, logout, session tracking
// Faraz Aamir | 25I-2069 | OOP Project | Spring 2026

#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include "MyString.h"
#include "User.h"
#include "Volume.h"

class SessionManager {
private:
    Volume* vfs;
    User* currentUser; // Currently logged in user (nullptr if none)

public:
    SessionManager(Volume* vfs);

    bool login(const MyString& username, const MyString& password);
    void logout();
    bool isLoggedIn() const;
    User* getCurrentUser() const;
    bool requireLogin() const; // Returns false and prints error if not logged in
};

#endif
