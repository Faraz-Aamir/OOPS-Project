// BeginnerContest.h — Beginner contest variant (easy-heavy)
// INHERITANCE: Extends Contest with 70% easy, 20% medium, 10% hard
// Faraz Aamir | 25I-2069 | OOP Project | Spring 2026

#ifndef BEGINNERCONTEST_H
#define BEGINNERCONTEST_H

#include "Contest.h"

class BeginnerContest : public Contest {
public:
    BeginnerContest(int id, const MyString& username, int timeLimitSeconds = 3600);
    ~BeginnerContest() override;

    // POLYMORPHISM: Each contest builds questions differently
    void buildQuestionSet(ProblemBank* bank) override;
    MyString getDifficultyProfile() const override;
    MyString getContestType() const override;
};

#endif
