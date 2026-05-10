// EvaluationEngine.h — Compile, run, and compare submissions
// Faraz Aamir   | 25I-2069 | OOP Project | Spring 2026
// Shawwal       | 25I-2182 |
// Farhan        | 25I-2073 |

#ifndef EVALUATIONENGINE_H
#define EVALUATIONENGINE_H

#include "MyString.h"
#include "Problem.h"
#include "Submission.h"
#include "IEvaluable.h"

class EvaluationEngine {
private:
    MyString tempDir;
    
    bool compileCode(const MyString& sourceFile, const MyString& outputFile) const;
    MyString runBinary(const MyString& binaryFile, const MyString& inputFile) const;
    bool compareOutput(const MyString& actual, const MyString& expected) const;
    void cleanup(const MyString& prefix) const;

public:
    EvaluationEngine(const MyString& tempDir = "/tmp/judge");

    // Evaluate a submission against a problem's test cases
    void evaluate(IEvaluable* submission, Problem* problem);
    
    // Get detailed results
    MyString getLastError() const;
};

#endif
