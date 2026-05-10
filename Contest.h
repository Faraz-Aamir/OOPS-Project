// Contest.h — Abstract contest base class (INHERITANCE + POLYMORPHISM)
// Different contest groups override buildQuestionSet() differently
// Faraz Aamir   | 25I-2069 | OOP Project | Spring 2026
// Shawwal       | 25I-2182 |
// Farhan        | 25I-2073 |

#ifndef CONTEST_H
#define CONTEST_H

#include "MyString.h"
#include "DynamicArray.h"
#include "Problem.h"
#include "Submission.h"
#include "ISerializable.h"
#include <ctime>

// Forward declaration
class ProblemBank;

class Contest : public ISerializable {
protected:
    int contestId;
    MyString username;                     // Which user owns this contest
    DynamicArray<Problem*> problems;       // Selected problems for this contest (aggregation)
    DynamicArray<Submission*> submissions; // User's submissions during contest (composition)
    int timeLimit;                         // Total time in seconds
    time_t startTime;                      // When contest started
    bool active;                           // Is contest currently running?
    bool completed;                        // Has contest ended?
    int score;                             // Final score
    DynamicArray<MyString> savedCode;      // Partial code for each problem (for save/resume)
    DynamicArray<int> attemptCounts;       // Wrong attempts per problem

public:
    Contest(int id, const MyString& username, int timeLimitSeconds);
    virtual ~Contest();

    // PURE VIRTUAL — each contest group builds question sets differently (POLYMORPHISM)
    virtual void buildQuestionSet(ProblemBank* bank) = 0;
    virtual MyString getDifficultyProfile() const = 0;
    virtual MyString getContestType() const = 0;

    // Contest lifecycle
    void start();
    void end();
    bool isActive() const;
    bool isCompleted() const;
    int getRemainingTime() const;
    bool hasTimeExpired() const;

    // Problem access
    int getProblemCount() const;
    Problem* getProblem(int index) const;
    const DynamicArray<Problem*>& getProblems() const;

    // Submission management
    void addSubmission(Submission* sub);
    const DynamicArray<Submission*>& getSubmissions() const;
    int getSubmissionCount() const;

    // Save/Resume
    void saveCode(int problemIndex, const MyString& code);
    MyString getSavedCode(int problemIndex) const;

    // Attempt tracking
    void incrementAttempts(int problemIndex);
    int getAttempts(int problemIndex) const;

    // Score
    void setScore(int s);
    int getScore() const;

    // Getters
    int getContestId() const;
    MyString getUsername() const;
    int getTimeLimit() const;
    time_t getStartTime() const;

    // ISerializable
    MyString serialize() const override;
    MyString getSerializationId() const override;
};

#endif
