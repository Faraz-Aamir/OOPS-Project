// ContestManager.cpp — Contest lifecycle implementation
// Faraz Aamir   | 25I-2069 | OOP Project | Spring 2026
// Shawwal       | 25I-2182 |
// Farhan        | 25I-2073 |

#include "ContestManager.h"

ContestManager::ContestManager(Volume* vfs, ProblemBank* problemBank)
    : vfs(vfs), problemBank(problemBank), nextContestId(1) {}

ContestManager::~ContestManager() {
    for (int i = 0; i < activeContests.size(); i++) {
        delete activeContests[i];
    }
}

// POLYMORPHISM: Creates the right contest type based on user's group assignment
Contest* ContestManager::createContest(User* user) {
    if (!user) return nullptr;

    MyString group = user->getContestGroup();
    Contest* contest = nullptr;

    // Polymorphic creation — each type builds questions differently
    if (group == "beginner") {
        contest = new BeginnerContest(nextContestId++, user->getUsername());
    } else if (group == "intermediate") {
        contest = new IntermediateContest(nextContestId++, user->getUsername());
    } else {
        contest = new AdvancedContest(nextContestId++, user->getUsername());
    }

    // POLYMORPHISM: buildQuestionSet() runs the correct variant
    contest->buildQuestionSet(problemBank);
    activeContests.push_back(contest);
    return contest;
}

bool ContestManager::startContest(Contest* contest) {
    if (!contest) return false;
    contest->start();
    return true;
}

Verdict ContestManager::submitSolution(Contest* contest, int problemIndex,
                                        const MyString& code, User* user) {
    if (!contest || !contest->isActive()) return Verdict::RUNTIME_ERROR;
    if (contest->hasTimeExpired()) {
        endContest(contest);
        return Verdict::TIME_LIMIT_EXCEEDED;
    }

    Problem* problem = contest->getProblem(problemIndex);
    if (!problem) return Verdict::RUNTIME_ERROR;

    // Create submission
    Submission* sub = new Submission(
        contest->getSubmissionCount() + 1,
        problem->getProblemId(),
        user->getUsername(),
        code
    );

    // Evaluate using the engine (polymorphic via IEvaluable interface)
    evalEngine.evaluate(sub, problem);

    // Track attempts
    if (sub->getVerdict() != Verdict::ACCEPTED) {
        contest->incrementAttempts(problemIndex);
    }

    contest->addSubmission(sub);

    // Save code to VFS for this contest
    MyString contestPath = MyString("users/") + user->getUsername() + "/contests/contest_"
                          + MyString::fromInt(contest->getContestId());
    MyString codePath = contestPath + "/problem_" + MyString::fromInt(problem->getProblemId()) + ".cpp";
    vfs->writeToVFS(codePath, code, user);

    return sub->getVerdict();
}

bool ContestManager::saveProgress(Contest* contest, int problemIndex, const MyString& code) {
    if (!contest) return false;
    contest->saveCode(problemIndex, code);

    // Also persist to VFS
    User* user = vfs->findUser(contest->getUsername());
    if (user) {
        MyString path = MyString("users/") + contest->getUsername() + "/contests/contest_"
                       + MyString::fromInt(contest->getContestId()) + "/progress.dat";
        vfs->writeToVFS(path, contest->serialize(), user);
    }
    return true;
}

int ContestManager::endContest(Contest* contest) {
    if (!contest) return 0;

    int score = ScoringEngine::calculateScore(contest);
    contest->setScore(score);
    contest->end();

    // Save final contest data to VFS
    User* user = vfs->findUser(contest->getUsername());
    if (user) {
        MyString metaPath = MyString("users/") + contest->getUsername() + "/contests/contest_"
                           + MyString::fromInt(contest->getContestId()) + "/meta.dat";
        vfs->writeToVFS(metaPath, contest->serialize(), user);
        user->addScore(score);
    }
    return score;
}

bool ContestManager::saveContestCode(Contest* contest, User* user) {
    if (!contest || !user) return false;
    std::cout << "  Contest code saved to your account." << std::endl;
    return true; // Code is already in VFS from submissions
}

bool ContestManager::discardContestCode(Contest* contest, User* user) {
    if (!contest || !user) return false;
    MyString contestPath = MyString("users/") + user->getUsername() + "/contests/contest_"
                          + MyString::fromInt(contest->getContestId());
    vfs->deleteFromVFS(contestPath, user);
    std::cout << "  Contest code discarded." << std::endl;
    return true;
}

Contest* ContestManager::resumeContest(User* user) {
    if (!user) return nullptr;
    for (int i = 0; i < activeContests.size(); i++) {
        if (activeContests[i]->getUsername() == user->getUsername() &&
            activeContests[i]->isActive()) {
            return activeContests[i];
        }
    }
    return nullptr;
}

Contest* ContestManager::getActiveContest(const MyString& username) const {
    for (int i = 0; i < activeContests.size(); i++) {
        if (activeContests[i]->getUsername() == username && activeContests[i]->isActive()) {
            return activeContests[i];
        }
    }
    return nullptr;
}

ProblemBank* ContestManager::getProblemBank() const { return problemBank; }
