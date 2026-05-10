// ContestManager.h — Contest lifecycle management
// Faraz Aamir   | 25I-2069 | OOP Project | Spring 2026
// Shawwal       | 25I-2182 |
// Farhan        | 25I-2073 |

#ifndef CONTESTMANAGER_H
#define CONTESTMANAGER_H

#include "MyString.h"
#include "DynamicArray.h"
#include "Contest.h"
#include "BeginnerContest.h"
#include "IntermediateContest.h"
#include "AdvancedContest.h"
#include "ProblemBank.h"
#include "EvaluationEngine.h"
#include "ScoringEngine.h"
#include "Volume.h"

class ContestManager {
private:
    Volume* vfs;
    ProblemBank* problemBank;
    EvaluationEngine evalEngine;
    DynamicArray<Contest*> activeContests; // Currently running contests
    int nextContestId;

public:
    ContestManager(Volume* vfs, ProblemBank* problemBank);
    ~ContestManager();

    // Create a contest for a user based on their assigned group
    Contest* createContest(User* user);

    // Start a contest
    bool startContest(Contest* contest);

    // Submit code for a problem in a contest
    Verdict submitSolution(Contest* contest, int problemIndex, const MyString& code, User* user);

    // Save progress mid-contest
    bool saveProgress(Contest* contest, int problemIndex, const MyString& code);

    // End contest (manual or auto when time expires)
    int endContest(Contest* contest);

    // Post-contest: save or discard contest code
    bool saveContestCode(Contest* contest, User* user);
    bool discardContestCode(Contest* contest, User* user);

    // Resume a saved contest
    Contest* resumeContest(User* user);

    // Get active contest for user
    Contest* getActiveContest(const MyString& username) const;

    // Problem bank access
    ProblemBank* getProblemBank() const;
};

#endif
