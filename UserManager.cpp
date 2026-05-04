// UserManager.cpp — User lifecycle management
// Faraz Aamir | 25I-2069 | OOP Project | Spring 2026

#include "UserManager.h"

UserManager::UserManager(Volume* vfs) : vfs(vfs) {}

User* UserManager::registerUser(const MyString& username, const MyString& email, const MyString& password) {
    // Validation
    if (username.empty() || email.empty() || password.empty()) {
        std::cout << "  ERROR: All fields are required." << std::endl;
        return nullptr;
    }
    if (vfs->findUser(username)) {
        std::cout << "  ERROR: Username '" << username << "' already taken." << std::endl;
        return nullptr;
    }
    if (vfs->findUserByEmail(email)) {
        std::cout << "  ERROR: Email '" << email << "' already registered." << std::endl;
        return nullptr;
    }

    int userId = vfs->getNextUserId();
    User* user = vfs->addOnlineJudgeUser(username, userId, email, password, UserRole::STANDARD);
    if (!user) return nullptr;

    // Auto-assign contest group
    assignContestGroup(user);

    // Send verification email
    emailService.sendVerificationEmail(email, username);

    // Save user profile to VFS
    MyString profilePath = MyString("users/") + username + "/profile.dat";
    vfs->writeToVFS(profilePath, user->serialize(), user);

    std::cout << "  User '" << username << "' registered successfully! Please verify your email." << std::endl;
    return user;
}

bool UserManager::verifyEmail(const MyString& email, const MyString& code) {
    if (emailService.verifyCode(email, code)) {
        User* user = vfs->findUserByEmail(email);
        if (user) {
            user->setVerified(true);
            // Update profile in VFS
            MyString profilePath = MyString("users/") + user->getUsername() + "/profile.dat";
            vfs->writeToVFS(profilePath, user->serialize(), user);
            std::cout << "  Email verified successfully! You can now log in." << std::endl;
            return true;
        }
    }
    std::cout << "  ERROR: Invalid verification code." << std::endl;
    return false;
}

bool UserManager::deleteAccount(const MyString& username) {
    User* user = vfs->findUser(username);
    if (!user) {
        std::cout << "  ERROR: User '" << username << "' not found." << std::endl;
        return false;
    }
    // Recursive VFS wipe + user removal
    return vfs->removeUser(username);
}

void UserManager::assignContestGroup(User* user) {
    if (!user) return;
    // Simple assignment based on user ID (cyclic: beginner, intermediate, advanced)
    int id = user->getUserId();
    if (id % 3 == 0) user->setContestGroup("beginner");
    else if (id % 3 == 1) user->setContestGroup("intermediate");
    else user->setContestGroup("advanced");
}

EmailService& UserManager::getEmailService() { return emailService; }
