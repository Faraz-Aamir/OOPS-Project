// EmailService.h — Simulated email verification
// Faraz Aamir   | 25I-2069 | OOP Project | Spring 2026
// Shawwal       | 25I-2182 |
// Farhan        | 25I-2073 |

#ifndef EMAILSERVICE_H
#define EMAILSERVICE_H

#include "MyString.h"
#include "HashMap.h"

class EmailService {
private:
    HashMap<MyString, MyString> pendingCodes; // email -> verification code

    MyString generateCode() const;

public:
    EmailService();

    // Send verification email (simulated — displays code on screen)
    MyString sendVerificationEmail(const MyString& email, const MyString& username);

    // Verify a code
    bool verifyCode(const MyString& email, const MyString& code);
};

#endif
