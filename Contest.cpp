// Contest.cpp — Abstract contest base implementation
// Faraz Aamir | 25I-2069 | OOP Project | Spring 2026

#include "Contest.h"

Contest::Contest(int id, const MyString& username, int timeLimitSeconds)
    : contestId(id), username(username), timeLimit(timeLimitSeconds),
      startTime(0), active(false), completed(false), score(0) {}

Contest::~Contest() {
    // Composition: contest owns its submissions
    for (int i = 0; i < submissions.size(); i++) {
        delete submissions[i];
    }
    // Problems are NOT owned (aggregation from ProblemBank)
}

void Contest::start() {
    startTime = time(nullptr);
    active = true;
    completed = false;
    // Initialize saved code and attempt arrays
    for (int i = 0; i < problems.size(); i++) {
        savedCode.push_back(MyString(""));
        attemptCounts.push_back(0);
    }
    std::cout << "  Contest " << contestId << " started for user " << username << "!" << std::endl;
}

void Contest::end() {
    active = false;
    completed = true;
    std::cout << "  Contest " << contestId << " ended. Score: " << score << std::endl;
}

bool Contest::isActive() const { return active && !hasTimeExpired(); }

bool Contest::isCompleted() const { return completed; }

int Contest::getRemainingTime() const {
    if (!active) return timeLimit;
    time_t now = time(nullptr);
    int elapsed = (int)(now - startTime);
    int remaining = timeLimit - elapsed;
    return (remaining > 0) ? remaining : 0;
}

bool Contest::hasTimeExpired() const {
    if (!active) return false;
    return getRemainingTime() <= 0;
}

int Contest::getProblemCount() const { return problems.size(); }
Problem* Contest::getProblem(int index) const {
    if (index < 0 || index >= problems.size()) return nullptr;
    return problems[index];
}
const DynamicArray<Problem*>& Contest::getProblems() const { return problems; }

void Contest::addSubmission(Submission* sub) {
    submissions.push_back(sub);
}

const DynamicArray<Submission*>& Contest::getSubmissions() const { return submissions; }
int Contest::getSubmissionCount() const { return submissions.size(); }

void Contest::saveCode(int problemIndex, const MyString& code) {
    if (problemIndex >= 0 && problemIndex < savedCode.size()) {
        savedCode[problemIndex] = code;
    }
}

MyString Contest::getSavedCode(int problemIndex) const {
    if (problemIndex >= 0 && problemIndex < savedCode.size()) {
        return savedCode[problemIndex];
    }
    return MyString("");
}

void Contest::incrementAttempts(int problemIndex) {
    if (problemIndex >= 0 && problemIndex < attemptCounts.size()) {
        attemptCounts[problemIndex]++;
    }
}

int Contest::getAttempts(int problemIndex) const {
    if (problemIndex >= 0 && problemIndex < attemptCounts.size()) {
        return attemptCounts[problemIndex];
    }
    return 0;
}

void Contest::setScore(int s) { score = s; }
int Contest::getScore() const { return score; }
int Contest::getContestId() const { return contestId; }
MyString Contest::getUsername() const { return username; }
int Contest::getTimeLimit() const { return timeLimit; }
time_t Contest::getStartTime() const { return startTime; }

MyString Contest::getSerializationId() const {
    return MyString("contest_") + MyString::fromInt(contestId);
}

MyString Contest::serialize() const {
    MyString result;
    result += MyString::fromInt(contestId) + "|";
    result += username + "|";
    result += getContestType() + "|";
    result += MyString::fromInt(timeLimit) + "|";
    result += MyString::fromLong((long)startTime) + "|";
    result += (active ? "1" : "0"); result += "|";
    result += (completed ? "1" : "0"); result += "|";
    result += MyString::fromInt(score) + "|";
    result += MyString::fromInt(problems.size()) + "|";
    for (int i = 0; i < problems.size(); i++) {
        result += MyString::fromInt(problems[i]->getProblemId());
        if (i < problems.size() - 1) result += ",";
    }
    result += "|";
    // Save code states
    for (int i = 0; i < savedCode.size(); i++) {
        result += savedCode[i];
        if (i < savedCode.size() - 1) result += "^^^";
    }
    return result;
}
