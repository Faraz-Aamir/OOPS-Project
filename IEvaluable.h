// IEvaluable.h — Abstract interface for code evaluation
// Shared evaluation contract for submission handling
// Faraz Aamir | 25I-2069 | OOP Project | Spring 2026

#ifndef IEVALUABLE_H
#define IEVALUABLE_H

#include "MyString.h"

// Verdict enum for submission results
enum class Verdict {
    PENDING,
    ACCEPTED,
    WRONG_ANSWER,
    COMPILATION_ERROR,
    RUNTIME_ERROR,
    TIME_LIMIT_EXCEEDED
};

// ABSTRACTION: Any object that can be evaluated against test cases
// implements this interface. The evaluation engine works with IEvaluable*
// without knowing the concrete submission type.
class IEvaluable {
public:
    virtual ~IEvaluable() {}

    // Get the source code to evaluate
    virtual MyString getSourceCode() const = 0;

    // Set the verdict after evaluation
    virtual void setVerdict(Verdict v) = 0;

    // Get current verdict
    virtual Verdict getVerdict() const = 0;

    // Get a human-readable verdict string
    static MyString verdictToString(Verdict v) {
        switch (v) {
            case Verdict::PENDING:              return MyString("PENDING");
            case Verdict::ACCEPTED:             return MyString("ACCEPTED");
            case Verdict::WRONG_ANSWER:         return MyString("WRONG_ANSWER");
            case Verdict::COMPILATION_ERROR:    return MyString("COMPILATION_ERROR");
            case Verdict::RUNTIME_ERROR:        return MyString("RUNTIME_ERROR");
            case Verdict::TIME_LIMIT_EXCEEDED:  return MyString("TIME_LIMIT_EXCEEDED");
        }
        return MyString("UNKNOWN");
    }
};

#endif
