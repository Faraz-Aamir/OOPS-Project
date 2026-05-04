// User.cpp — User model implementation
// Faraz Aamir | 25I-2069 | OOP Project | Spring 2026

#include "User.h"

// Simple hash: shift each character by a fixed amount and XOR
MyString User::hashPassword(const MyString& password) {
    MyString hashed;
    for (int i = 0; i < password.length(); i++) {
        char c = password[i];
        // Simple transformation: XOR with position and shift
        char hc = ((c ^ (i + 42)) + 13) % 126;
        if (hc < 33) hc += 33; // Keep printable
        hashed += hc;
    }
    return hashed;
}

// VFS-style constructor
User::User(const MyString& username, int userId, const MyString& homeDir,
           UserRole role, const MyString& groupName)
    : username(username), userId(userId), homeDir(homeDir),
      role(role), groupName(groupName), email(""), passwordHash(""),
      verified(false), loggedIn(false), contestGroup("beginner"), totalScore(0) {}

// Full Online Judge constructor
User::User(const MyString& username, int userId, const MyString& email,
           const MyString& password, const MyString& homeDir,
           UserRole role, const MyString& groupName)
    : username(username), userId(userId), homeDir(homeDir),
      role(role), groupName(groupName), email(email),
      passwordHash(hashPassword(password)),
      verified(false), loggedIn(false), contestGroup("beginner"), totalScore(0) {}

// --- VFS Getters ---
MyString User::getUsername() const { return username; }
int User::getUserId() const { return userId; }
MyString User::getHomeDir() const { return homeDir; }
UserRole User::getRole() const { return role; }
MyString User::getGroupName() const { return groupName; }
bool User::isSuperUser() const { return role == UserRole::SUPERUSER; }

// --- Online Judge Getters/Setters ---
MyString User::getEmail() const { return email; }
bool User::isVerified() const { return verified; }
void User::setVerified(bool v) { verified = v; }
bool User::isLoggedIn() const { return loggedIn; }
void User::setLoggedIn(bool v) { loggedIn = v; }
MyString User::getContestGroup() const { return contestGroup; }
void User::setContestGroup(const MyString& group) { contestGroup = group; }
int User::getTotalScore() const { return totalScore; }
void User::addScore(int score) { totalScore += score; }

bool User::checkPassword(const MyString& password) const {
    return hashPassword(password) == passwordHash;
}

// Serialize user data for VFS storage
// Format: username|userId|email|passwordHash|homeDir|role|group|verified|contestGroup|totalScore
MyString User::serialize() const {
    MyString result;
    result += username;     result += "|";
    result += MyString::fromInt(userId);  result += "|";
    result += email;        result += "|";
    result += passwordHash; result += "|";
    result += homeDir;      result += "|";
    result += (role == UserRole::SUPERUSER ? "superuser" : "standard"); result += "|";
    result += groupName;    result += "|";
    result += (verified ? "1" : "0"); result += "|";
    result += contestGroup; result += "|";
    result += MyString::fromInt(totalScore);
    return result;
}

// Deserialize from VFS stored string
User* User::deserialize(const MyString& data) {
    MyString parts[12];
    int count = data.split('|', parts, 12);
    if (count < 10) return nullptr;

    MyString uname = parts[0];
    int uid = parts[1].toInt();
    MyString em = parts[2];
    MyString pwHash = parts[3];
    MyString hdir = parts[4];
    UserRole r = (parts[5] == "superuser") ? UserRole::SUPERUSER : UserRole::STANDARD;
    MyString grp = parts[6];
    bool ver = (parts[7] == "1");
    MyString cgrp = parts[8];
    int score = parts[9].toInt();

    // Use VFS constructor and set fields manually (password is already hashed)
    User* user = new User(uname, uid, hdir, r, grp);
    user->email = em;
    user->passwordHash = pwHash;
    user->verified = ver;
    user->contestGroup = cgrp;
    user->totalScore = score;
    return user;
}

std::ostream& operator<<(std::ostream& os, const User& user) {
    os << "User[" << user.username << ", id=" << user.userId
       << ", role=" << (user.role == UserRole::SUPERUSER ? "root" : "standard")
       << ", group=" << user.groupName
       << ", email=" << user.email
       << ", verified=" << (user.verified ? "yes" : "no")
       << ", contest=" << user.contestGroup
       << ", score=" << user.totalScore << "]";
    return os;
}
