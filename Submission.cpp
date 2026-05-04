// Submission.cpp — Submission implementation
// Faraz Aamir | 25I-2069 | OOP Project | Spring 2026

#include "Submission.h"
#include "DynamicArray.h"

Submission::Submission() : submissionId(0), problemId(0), verdict(Verdict::PENDING),
    submittedAt(0), testsPassed(0), totalTests(0) {}

Submission::Submission(int subId, int probId, const MyString& username, const MyString& code)
    : submissionId(subId), problemId(probId), username(username), sourceCode(code),
      verdict(Verdict::PENDING), testsPassed(0), totalTests(0) {
    submittedAt = time(nullptr);
}

int Submission::getSubmissionId() const { return submissionId; }
int Submission::getProblemId() const { return problemId; }
MyString Submission::getUsername() const { return username; }
time_t Submission::getSubmittedAt() const { return submittedAt; }
int Submission::getTestsPassed() const { return testsPassed; }
int Submission::getTotalTests() const { return totalTests; }
void Submission::setTestResults(int passed, int total) { testsPassed = passed; totalTests = total; }

MyString Submission::getSourceCode() const { return sourceCode; }
void Submission::setVerdict(Verdict v) { verdict = v; }
Verdict Submission::getVerdict() const { return verdict; }

MyString Submission::getSerializationId() const {
    return MyString("submission_") + MyString::fromInt(submissionId);
}

MyString Submission::serialize() const {
    MyString result;
    result += MyString::fromInt(submissionId) + "|";
    result += MyString::fromInt(problemId) + "|";
    result += username + "|";
    result += MyString::fromInt((int)verdict) + "|";
    result += MyString::fromLong((long)submittedAt) + "|";
    result += MyString::fromInt(testsPassed) + "|";
    result += MyString::fromInt(totalTests) + "|";
    result += sourceCode;
    return result;
}

Submission* Submission::deserialize(const MyString& data) {
    // Find first 7 pipes
    DynamicArray<int> pipePositions;
    for (int i = 0; i < data.length(); i++) {
        if (data[i] == '|') {
            pipePositions.push_back(i);
            if (pipePositions.size() == 7) break;
        }
    }
    if (pipePositions.size() < 7) return nullptr;

    Submission* s = new Submission();
    s->submissionId = data.substr(0, pipePositions[0]).toInt();
    s->problemId = data.substr(pipePositions[0] + 1, pipePositions[1] - pipePositions[0] - 1).toInt();
    s->username = data.substr(pipePositions[1] + 1, pipePositions[2] - pipePositions[1] - 1);
    s->verdict = (Verdict)data.substr(pipePositions[2] + 1, pipePositions[3] - pipePositions[2] - 1).toInt();
    s->submittedAt = (time_t)atol(data.substr(pipePositions[3] + 1, pipePositions[4] - pipePositions[3] - 1).c_str());
    s->testsPassed = data.substr(pipePositions[4] + 1, pipePositions[5] - pipePositions[4] - 1).toInt();
    s->totalTests = data.substr(pipePositions[5] + 1, pipePositions[6] - pipePositions[5] - 1).toInt();
    s->sourceCode = data.substr(pipePositions[6] + 1);
    return s;
}
