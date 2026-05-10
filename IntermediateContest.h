// IntermediateContest.h — Intermediate contest variant (balanced)
// INHERITANCE: Extends Contest with 20% easy, 50% medium, 30% hard
// Faraz Aamir   | 25I-2069 | OOP Project | Spring 2026
// Shawwal       | 25I-2182 |
// Farhan        | 25I-2073 |

#ifndef INTERMEDIATECONTEST_H
#define INTERMEDIATECONTEST_H

#include "Contest.h"

class IntermediateContest : public Contest {
public:
    IntermediateContest(int id, const MyString& username, int timeLimitSeconds = 3600);
    ~IntermediateContest() override;

    void buildQuestionSet(ProblemBank* bank) override;
    MyString getDifficultyProfile() const override;
    MyString getContestType() const override;
};

#endif
