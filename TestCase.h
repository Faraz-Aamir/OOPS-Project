// TestCase.h — Input/output pair for a problem
// Faraz Aamir | 25I-2069 | OOP Project | Spring 2026

#ifndef TESTCASE_H
#define TESTCASE_H

#include "MyString.h"

class TestCase {
private:
    MyString input;
    MyString expectedOutput;

public:
    TestCase();
    TestCase(const MyString& input, const MyString& expectedOutput);

    MyString getInput() const;
    MyString getExpectedOutput() const;

    MyString serialize() const;
    static TestCase deserialize(const MyString& data);
};

#endif
