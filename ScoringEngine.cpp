// ScoringEngine.cpp — Score calculation implementation
// Faraz Aamir   | 25I-2069 | OOP Project | Spring 2026
// Shawwal       | 25I-2182 |
// Farhan        | 25I-2073 |

#include "ScoringEngine.h"
#include <ctime>

int ScoringEngine::calculateProblemScore(int timeUsedSeconds, int totalTimeSeconds, int wrongAttempts, bool solved) {
    if (!solved) return 0;

    int basePoints = 100;
    
    // TimeFactor: rewards speed (30% min, 100% max)
    double timeRatio = (double)timeUsedSeconds / (double)totalTimeSeconds;
    double timeFactor = 1.0 - timeRatio * 0.7;
    if (timeFactor < 0.3) timeFactor = 0.3;

    // AttemptPenalty: 10% per wrong attempt (50% min)
    double attemptPenalty = 1.0 - wrongAttempts * 0.1;
    if (attemptPenalty < 0.5) attemptPenalty = 0.5;

    return (int)(basePoints * timeFactor * attemptPenalty);
}

int ScoringEngine::calculateScore(Contest* contest) {
    if (!contest) return 0;

    int totalScore = 0;
    int totalTime = contest->getTimeLimit();
    time_t startTime = contest->getStartTime();
    time_t now = time(nullptr);
    int timeUsed = (int)(now - startTime);
    if (timeUsed > totalTime) timeUsed = totalTime;

    const DynamicArray<Submission*>& subs = contest->getSubmissions();
    const DynamicArray<Problem*>& probs = contest->getProblems();

    // For each problem, find if it was solved (last accepted submission)
    for (int i = 0; i < probs.size(); i++) {
        bool solved = false;
        for (int j = 0; j < subs.size(); j++) {
            if (subs[j]->getProblemId() == probs[i]->getProblemId() &&
                subs[j]->getVerdict() == Verdict::ACCEPTED) {
                solved = true;
                break;
            }
        }
        int attempts = contest->getAttempts(i);
        totalScore += calculateProblemScore(timeUsed, totalTime, attempts, solved);
    }

    return totalScore;
}
