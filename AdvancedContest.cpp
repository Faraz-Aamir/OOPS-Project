// AdvancedContest.cpp — Advanced: 10% easy, 30% medium, 60% hard
// Faraz Aamir   | 25I-2069 | OOP Project | Spring 2026
// Shawwal       | 25I-2182 |
// Farhan        | 25I-2073 |

#include "AdvancedContest.h"
#include "ProblemBank.h"

AdvancedContest::AdvancedContest(int id, const MyString& username, int timeLimitSeconds)
    : Contest(id, username, timeLimitSeconds) {}

AdvancedContest::~AdvancedContest() {}

void AdvancedContest::buildQuestionSet(ProblemBank* bank) {
    // 1 easy, 3 medium, 6 hard = 10 problems
    DynamicArray<Problem*> easy = bank->getRandomByDifficulty(1, 1);
    DynamicArray<Problem*> med = bank->getRandomByDifficulty(3, 3);
    DynamicArray<Problem*> hard4 = bank->getRandomByDifficulty(4, 3);
    DynamicArray<Problem*> hard5 = bank->getRandomByDifficulty(5, 3);

    for (int i = 0; i < easy.size(); i++) problems.push_back(easy[i]);
    for (int i = 0; i < med.size(); i++) problems.push_back(med[i]);
    for (int i = 0; i < hard4.size(); i++) problems.push_back(hard4[i]);
    for (int i = 0; i < hard5.size(); i++) problems.push_back(hard5[i]);
}

MyString AdvancedContest::getDifficultyProfile() const {
    return MyString("Advanced (10% easy, 30% medium, 60% hard)");
}

MyString AdvancedContest::getContestType() const {
    return MyString("advanced");
}
