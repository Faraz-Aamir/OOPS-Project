// TestCase.cpp — TestCase implementation
// Faraz Aamir | 25I-2069 | OOP Project | Spring 2026

#include "TestCase.h"

TestCase::TestCase() : input(""), expectedOutput("") {}

TestCase::TestCase(const MyString& input, const MyString& expectedOutput)
    : input(input), expectedOutput(expectedOutput) {}

MyString TestCase::getInput() const { return input; }
MyString TestCase::getExpectedOutput() const { return expectedOutput; }

MyString TestCase::serialize() const {
    // Use ~~~ as delimiter between input and output (unlikely in test data)
    return input + "~~~" + expectedOutput;
}

TestCase TestCase::deserialize(const MyString& data) {
    int pos = data.find("~~~");
    if (pos == -1) return TestCase();
    MyString inp = data.substr(0, pos);
    MyString out = data.substr(pos + 3);
    return TestCase(inp, out);
}
