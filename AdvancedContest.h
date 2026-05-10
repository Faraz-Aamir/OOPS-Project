// AdvancedContest.h — Advanced contest variant (hard-heavy)
// INHERITANCE: Extends Contest with 10% easy, 30% medium, 60% hard
// Faraz Aamir   | 25I-2069 | OOP Project | Spring 2026
// Shawwal       | 25I-2182 |
// Farhan        | 25I-2073 |

#ifndef ADVANCEDCONTEST_H
#define ADVANCEDCONTEST_H

#include "Contest.h"

class AdvancedContest : public Contest {
public:
    AdvancedContest(int id, const MyString& username, int timeLimitSeconds = 3600);
    ~AdvancedContest() override;

    void buildQuestionSet(ProblemBank* bank) override;
    MyString getDifficultyProfile() const override;
    MyString getContestType() const override;
};

#endif
