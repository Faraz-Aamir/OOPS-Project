// ProblemBank.h — Problem storage, loading, and filtering
// Faraz Aamir | 25I-2069 | OOP Project | Spring 2026

#ifndef PROBLEMBANK_H
#define PROBLEMBANK_H

#include "MyString.h"
#include "DynamicArray.h"
#include "Problem.h"
#include "Volume.h"

class ProblemBank {
private:
    DynamicArray<Problem*> allProblems;
    Volume* vfs;

public:
    ProblemBank(Volume* vfs);
    ~ProblemBank();

    // Seed all 50 problems into the bank and VFS
    void seedProblems();

    // Load problems from VFS (on startup)
    void loadFromVFS();

    // Get problems by difficulty
    DynamicArray<Problem*> getByDifficulty(int difficulty) const;

    // Get random problems from a specific difficulty (uses simple randomization)
    DynamicArray<Problem*> getRandomByDifficulty(int difficulty, int count) const;

    // Get all problems
    const DynamicArray<Problem*>& getAllProblems() const;

    // Get problem by ID
    Problem* getProblemById(int id) const;

    // Total count
    int getTotalCount() const;
    int getCountByDifficulty(int difficulty) const;
};

#endif
