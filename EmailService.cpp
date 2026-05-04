// EmailService.cpp — Simulated email verification implementation
// Faraz Aamir | 25I-2069 | OOP Project | Spring 2026

#include "EmailService.h"
#include <cstdlib>
#include <ctime>

EmailService::EmailService() {}

MyString EmailService::generateCode() const {
    // Generate a 6-digit verification code
    int code = 100000 + (rand() % 900000);
    return MyString::fromInt(code);
}

MyString EmailService::sendVerificationEmail(const MyString& email, const MyString& username) {
    MyString code = generateCode();
    pendingCodes.put(email, code);

    std::cout << "\n  ============================================" << std::endl;
    std::cout << "  |  EMAIL VERIFICATION (Simulated)          |" << std::endl;
    std::cout << "  |  To: " << email << std::endl;
    std::cout << "  |  Dear " << username << "," << std::endl;
    std::cout << "  |  Your verification code is: " << code << std::endl;
    std::cout << "  |  Please enter this code to activate your" << std::endl;
    std::cout << "  |  account on the Online Judge platform.   |" << std::endl;
    std::cout << "  ============================================\n" << std::endl;

    return code;
}

bool EmailService::verifyCode(const MyString& email, const MyString& code) {
    MyString* storedCode = pendingCodes.get(email);
    if (!storedCode) return false;
    if (*storedCode == code) {
        pendingCodes.remove(email);
        return true;
    }
    return false;
}
