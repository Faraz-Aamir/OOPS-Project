// OnlineJudge.h — Main application controller
// Wires all layers together; includes disk persistence on shutdown/startup
// Faraz Aamir   | 25I-2069 | OOP Project | Spring 2026
// Shawwal       | 25I-2182 |
// Farhan        | 25I-2073 |

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

    // State file path for disk persistence
    static const char* STATE_FILE;

public:
    OnlineJudge();
    ~OnlineJudge();

    void initialize(); // Set up VFS, seed problems, load saved state
    void shutdown();   // Save state to disk before exit
    void run(int port = 8080); // Start HTTP server

    // API handler — called by HttpServer
    MyString handleAPIRequest(const MyString& path, const MyString& method, const MyString& body);

    // Getters for components
    Volume* getVFS() const;
    SessionManager* getSessionManager() const;
    ContestManager* getContestManager() const;
};

#endif
