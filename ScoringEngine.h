// ScoringEngine.h — Score calculation
// Faraz Aamir   | 25I-2069 | OOP Project | Spring 2026
// Shawwal       | 25I-2182 |
// Farhan        | 25I-2073 |

#ifndef SCORINGENGINE_H
#define SCORINGENGINE_H

#include "Contest.h"

class ScoringEngine {
public:
    // Score = BasePoints × TimeFactor × AttemptPenalty
    // BasePoints = 100 per solved problem
    // TimeFactor = max(0.3, 1.0 - (timeUsed/totalTime) * 0.7)
    // AttemptPenalty = max(0.5, 1.0 - wrongAttempts * 0.1)
    static int calculateScore(Contest* contest);
    static int calculateProblemScore(int timeUsedSeconds, int totalTimeSeconds, int wrongAttempts, bool solved);
};

#endif
