// Problem.cpp — Problem implementation
// Faraz Aamir   | 25I-2069 | OOP Project | Spring 2026
// Shawwal       | 25I-2182 |
// Farhan        | 25I-2073 |

#include "Problem.h"

Problem::Problem() : problemId(0), difficulty(1) {}

Problem::Problem(int id, const MyString& title, const MyString& description,
                 const MyString& constraints, int difficulty,
                 const MyString& sampleInput, const MyString& sampleOutput,
                 const MyString& codeTemplate)
    : problemId(id), title(title), description(description),
      constraints(constraints), difficulty(difficulty),
      sampleInput(sampleInput), sampleOutput(sampleOutput),
      codeTemplate(codeTemplate) {}

int Problem::getProblemId() const { return problemId; }
MyString Problem::getTitle() const { return title; }
MyString Problem::getDescription() const { return description; }
MyString Problem::getConstraints() const { return constraints; }
int Problem::getDifficulty() const { return difficulty; }
MyString Problem::getSampleInput() const { return sampleInput; }
MyString Problem::getSampleOutput() const { return sampleOutput; }
MyString Problem::getCodeTemplate() const { return codeTemplate; }
const DynamicArray<TestCase>& Problem::getTestCases() const { return testCases; }
int Problem::getTestCaseCount() const { return testCases.size(); }

void Problem::addTestCase(const TestCase& tc) {
    testCases.push_back(tc);
}

MyString Problem::getSerializationId() const {
    return MyString("problem_") + MyString::fromInt(problemId);
}

MyString Problem::serialize() const {
    MyString result;
    result += MyString::fromInt(problemId) + "|||";
    result += title + "|||";
    result += description + "|||";
    result += constraints + "|||";
    result += MyString::fromInt(difficulty) + "|||";
    result += sampleInput + "|||";
    result += sampleOutput + "|||";
    result += codeTemplate + "|||";
    result += MyString::fromInt(testCases.size());
    for (int i = 0; i < testCases.size(); i++) {
        result += "|||" + testCases[i].serialize();
    }
    return result;
}

Problem* Problem::deserialize(const MyString& data) {
    // Split by |||
    MyString parts[64];
    data.split('|', parts, 64);
    // "|||" splits into parts with empty strings between, so we need to handle this
    // Actually let's use a simpler approach — find "|||" positions
    DynamicArray<MyString> sections;
    int start = 0;
    while (start < data.length()) {
        int pos = data.find("|||", start);
        if (pos == -1) {
            sections.push_back(data.substr(start));
            break;
        }
        sections.push_back(data.substr(start, pos - start));
        start = pos + 3;
    }

    if (sections.size() < 9) return nullptr;

    Problem* p = new Problem();
    p->problemId = sections[0].toInt();
    p->title = sections[1];
    p->description = sections[2];
    p->constraints = sections[3];
    p->difficulty = sections[4].toInt();
    p->sampleInput = sections[5];
    p->sampleOutput = sections[6];
    p->codeTemplate = sections[7];
    int tcCount = sections[8].toInt();
    for (int i = 0; i < tcCount && (9 + i) < sections.size(); i++) {
        p->testCases.push_back(TestCase::deserialize(sections[9 + i]));
    }
    return p;
}
