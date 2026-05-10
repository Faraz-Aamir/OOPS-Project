// IntermediateContest.cpp — Intermediate: 20% easy, 50% medium, 30% hard
// Faraz Aamir   | 25I-2069 | OOP Project | Spring 2026
// Shawwal       | 25I-2182 |
// Farhan        | 25I-2073 |

#include "IntermediateContest.h"
#include "ProblemBank.h"

IntermediateContest::IntermediateContest(int id, const MyString& username, int timeLimitSeconds)
    : Contest(id, username, timeLimitSeconds) {}

IntermediateContest::~IntermediateContest() {}

void IntermediateContest::buildQuestionSet(ProblemBank* bank) {
    // 2 easy, 5 medium, 3 hard = 10 problems
    DynamicArray<Problem*> easy = bank->getRandomByDifficulty(1, 1);
    DynamicArray<Problem*> easy2 = bank->getRandomByDifficulty(2, 1);
    DynamicArray<Problem*> med = bank->getRandomByDifficulty(3, 5);
    DynamicArray<Problem*> hard = bank->getRandomByDifficulty(4, 2);
    DynamicArray<Problem*> hard2 = bank->getRandomByDifficulty(5, 1);

    for (int i = 0; i < easy.size(); i++) problems.push_back(easy[i]);
    for (int i = 0; i < easy2.size(); i++) problems.push_back(easy2[i]);
    for (int i = 0; i < med.size(); i++) problems.push_back(med[i]);
    for (int i = 0; i < hard.size(); i++) problems.push_back(hard[i]);
    for (int i = 0; i < hard2.size(); i++) problems.push_back(hard2[i]);
}

MyString IntermediateContest::getDifficultyProfile() const {
    return MyString("Intermediate (20% easy, 50% medium, 30% hard)");
}

MyString IntermediateContest::getContestType() const {
    return MyString("intermediate");
}
