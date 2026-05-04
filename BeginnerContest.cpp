// BeginnerContest.cpp — Beginner contest: 70% easy, 20% medium, 10% hard
// Faraz Aamir | 25I-2069 | OOP Project | Spring 2026

#include "BeginnerContest.h"
#include "ProblemBank.h"

BeginnerContest::BeginnerContest(int id, const MyString& username, int timeLimitSeconds)
    : Contest(id, username, timeLimitSeconds) {}

BeginnerContest::~BeginnerContest() {}

void BeginnerContest::buildQuestionSet(ProblemBank* bank) {
    // 70% easy (diff 1-2), 20% medium (diff 3), 10% hard (diff 4-5)
    // Total 10 problems: 7 easy, 2 medium, 1 hard
    DynamicArray<Problem*> easy1 = bank->getRandomByDifficulty(1, 4);
    DynamicArray<Problem*> easy2 = bank->getRandomByDifficulty(2, 3);
    DynamicArray<Problem*> med = bank->getRandomByDifficulty(3, 2);
    DynamicArray<Problem*> hard = bank->getRandomByDifficulty(4, 1);

    for (int i = 0; i < easy1.size(); i++) problems.push_back(easy1[i]);
    for (int i = 0; i < easy2.size(); i++) problems.push_back(easy2[i]);
    for (int i = 0; i < med.size(); i++) problems.push_back(med[i]);
    for (int i = 0; i < hard.size(); i++) problems.push_back(hard[i]);
}

MyString BeginnerContest::getDifficultyProfile() const {
    return MyString("Beginner (70% easy, 20% medium, 10% hard)");
}

MyString BeginnerContest::getContestType() const {
    return MyString("beginner");
}
