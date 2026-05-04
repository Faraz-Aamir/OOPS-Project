// User.h — System user model
// Ported from Assignment 3 + extended for Online Judge (email, password, session)
// Faraz Aamir | 25I-2069 | OOP Project | Spring 2026

#ifndef USER_H
#define USER_H

#include "MyString.h"
#include <iostream>

enum class UserRole { STANDARD, SUPERUSER };

class User {
private:
    MyString username;
    int userId;
    MyString homeDir;
    UserRole role;
    MyString groupName;

    // --- Online Judge extensions ---
    MyString email;
    MyString passwordHash;    // Simple hash of password (not plaintext)
    bool verified;            // Email verified?
    bool loggedIn;            // Currently in an active session?
    MyString contestGroup;    // "beginner", "intermediate", "advanced"
    int totalScore;

    // Simple password hashing (shift cipher — not cryptographic, but sufficient for demo)
    static MyString hashPassword(const MyString& password);

public:
    // VFS-style constructor (backward compatible)
    User(const MyString& username, int userId, const MyString& homeDir,
         UserRole role, const MyString& groupName = "users");

    // Online Judge constructor (full)
    User(const MyString& username, int userId, const MyString& email,
         const MyString& password, const MyString& homeDir,
         UserRole role, const MyString& groupName = "users");

    // --- VFS Getters ---
    MyString getUsername() const;
    int getUserId() const;
    MyString getHomeDir() const;
    UserRole getRole() const;
    MyString getGroupName() const;
    bool isSuperUser() const;

    // --- Online Judge Getters/Setters ---
    MyString getEmail() const;
    bool isVerified() const;
    void setVerified(bool v);
    bool isLoggedIn() const;
    void setLoggedIn(bool v);
    MyString getContestGroup() const;
    void setContestGroup(const MyString& group);
    int getTotalScore() const;
    void addScore(int score);

    // Password verification
    bool checkPassword(const MyString& password) const;

    // Serialize to string for VFS storage
    MyString serialize() const;

    // Deserialize from string (static factory)
    static User* deserialize(const MyString& data);

    friend std::ostream& operator<<(std::ostream& os, const User& user);
};

#endif
