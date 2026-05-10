// SessionManager.cpp
// Faraz Aamir   | 25I-2069 | OOP Project | Spring 2026
// Shawwal       | 25I-2182 |
// Farhan        | 25I-2073 |

#include "SessionManager.h"

SessionManager::SessionManager(Volume* vfs) : vfs(vfs), currentUser(nullptr) {}

bool SessionManager::login(const MyString& username, const MyString& password) {
    User* user = vfs->findUser(username);
    if (!user) {
        std::cout << "  ERROR: User '" << username << "' not found." << std::endl;
        return false;
    }
    if (!user->isVerified()) {
        std::cout << "  ERROR: Account not verified. Please verify your email first." << std::endl;
        return false;
    }
    if (!user->checkPassword(password)) {
        std::cout << "  ERROR: Incorrect password." << std::endl;
        return false;
    }
    if (currentUser) {
        currentUser->setLoggedIn(false);
    }
    currentUser = user;
    currentUser->setLoggedIn(true);
    std::cout << "  Logged in as '" << username << "'." << std::endl;
    return true;
}

void SessionManager::logout() {
    if (currentUser) {
        std::cout << "  User '" << currentUser->getUsername() << "' logged out." << std::endl;
        currentUser->setLoggedIn(false);
        currentUser = nullptr;
    }
}

bool SessionManager::isLoggedIn() const { return currentUser != nullptr; }
User* SessionManager::getCurrentUser() const { return currentUser; }

bool SessionManager::requireLogin() const {
    if (!currentUser) {
        std::cout << "  ERROR: You must be logged in to perform this action." << std::endl;
        return false;
    }
    return true;
}
