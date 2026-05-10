// Submission.h — Code submission with verdict
// Faraz Aamir   | 25I-2069 | OOP Project | Spring 2026
// Shawwal       | 25I-2182 |
// Farhan        | 25I-2073 |

#ifndef SUBMISSION_H
#define SUBMISSION_H

#include "MyString.h"
#include "ISerializable.h"
#include "IEvaluable.h"
#include <ctime>

class Submission : public ISerializable, public IEvaluable {
private:
    int submissionId;
    int problemId;
    MyString username;
    MyString sourceCode;
    Verdict verdict;
    time_t submittedAt;
    int testsPassed;
    int totalTests;

public:
    Submission();
    Submission(int subId, int probId, const MyString& username, const MyString& code);

    int getSubmissionId() const;
    int getProblemId() const;
    MyString getUsername() const;
    time_t getSubmittedAt() const;
    int getTestsPassed() const;
    int getTotalTests() const;
    void setTestResults(int passed, int total);

    // IEvaluable interface
    MyString getSourceCode() const override;
    void setVerdict(Verdict v) override;
    Verdict getVerdict() const override;

    // ISerializable interface
    MyString serialize() const override;
    MyString getSerializationId() const override;
    static Submission* deserialize(const MyString& data);
};

#endif
