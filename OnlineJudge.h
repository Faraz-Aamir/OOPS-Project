// OnlineJudge.h — Main application controller
// Wires all layers together
// Faraz Aamir | 25I-2069 | OOP Project | Spring 2026

#ifndef ONLINEJUDGE_H
#define ONLINEJUDGE_H

#include "MyString.h"
#include "Volume.h"
#include "ProblemBank.h"
#include "UserManager.h"
#include "SessionManager.h"
#include "ContestManager.h"

class OnlineJudge {
private:
    Volume* vfs;
    ProblemBank* problemBank;
    UserManager* userManager;
    SessionManager* sessionManager;
    ContestManager* contestManager;

    // JSON helper
    MyString jsonResponse(const MyString& status, const MyString& message, const MyString& data = "") const;

public:
    OnlineJudge();
    ~OnlineJudge();

    void initialize(); // Set up VFS, seed problems
    void run(int port = 8080); // Start HTTP server

    // API handler — called by HttpServer
    MyString handleAPIRequest(const MyString& path, const MyString& method, const MyString& body);

    // Getters for components
    Volume* getVFS() const;
    SessionManager* getSessionManager() const;
    ContestManager* getContestManager() const;
};

#endif
