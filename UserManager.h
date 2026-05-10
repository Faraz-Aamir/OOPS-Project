// UserManager.h — Registration, verification, deletion
// Faraz Aamir   | 25I-2069 | OOP Project | Spring 2026
// Shawwal       | 25I-2182 |
// Farhan        | 25I-2073 |

#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "MyString.h"
#include "Volume.h"
#include "EmailService.h"

class UserManager {
private:
    Volume* vfs;
    EmailService emailService;

public:
    UserManager(Volume* vfs);

    // Register new user — creates VFS space, sends verification email
    User* registerUser(const MyString& username, const MyString& email, const MyString& password);

    // Verify email with code
    bool verifyEmail(const MyString& email, const MyString& code);

    // Delete user account — recursive VFS wipe
    bool deleteAccount(const MyString& username);

    // Auto-assign contest group based on some criteria
    void assignContestGroup(User* user);

    EmailService& getEmailService();
};

#endif
