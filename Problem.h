// Problem.h — Competitive programming problem
// Faraz Aamir   | 25I-2069 | OOP Project | Spring 2026
// Shawwal       | 25I-2182 |
// Farhan        | 25I-2073 |

#ifndef PROBLEM_H
#define PROBLEM_H

#include "MyString.h"
#include "DynamicArray.h"
#include "TestCase.h"
#include "ISerializable.h"

class Problem : public ISerializable {
private:
    int problemId;
    MyString title;
    MyString description;
    MyString constraints;
    int difficulty;              // 1-5
    MyString sampleInput;
    MyString sampleOutput;
    MyString codeTemplate;       // Template code to wrap user solution
    DynamicArray<TestCase> testCases;

public:
    Problem();
    Problem(int id, const MyString& title, const MyString& description,
            const MyString& constraints, int difficulty,
            const MyString& sampleInput, const MyString& sampleOutput,
            const MyString& codeTemplate);

    // Getters
    int getProblemId() const;
    MyString getTitle() const;
    MyString getDescription() const;
    MyString getConstraints() const;
    int getDifficulty() const;
    MyString getSampleInput() const;
    MyString getSampleOutput() const;
    MyString getCodeTemplate() const;
    const DynamicArray<TestCase>& getTestCases() const;
    int getTestCaseCount() const;

    // Add test case
    void addTestCase(const TestCase& tc);

    // ISerializable
    MyString serialize() const override;
    MyString getSerializationId() const override;

    // Static deserialize
    static Problem* deserialize(const MyString& data);
};

#endif
