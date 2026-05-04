// EvaluationEngine.cpp — Code evaluation via system() calls
// Faraz Aamir | 25I-2069 | OOP Project | Spring 2026

#include "EvaluationEngine.h"
#include <cstdlib>
#include <cstdio>
#include <fstream>

EvaluationEngine::EvaluationEngine(const MyString& tempDir) : tempDir(tempDir) {
    // Ensure temp directory exists
    MyString cmd = MyString("mkdir -p ") + tempDir;
    system(cmd.c_str());
}

bool EvaluationEngine::compileCode(const MyString& sourceFile, const MyString& outputFile) const {
    MyString cmd = MyString("g++ -o ") + outputFile + " " + sourceFile + " 2>/dev/null";
    int result = system(cmd.c_str());
    return (result == 0);
}

MyString EvaluationEngine::runBinary(const MyString& binaryFile, const MyString& inputFile) const {
    MyString outputFile = tempDir + "/run_output.txt";
    // Run with timeout of 5 seconds to prevent infinite loops
    MyString cmd = MyString("timeout 5 ") + binaryFile + " < " + inputFile + " > " + outputFile + " 2>/dev/null";
    int result = system(cmd.c_str());
    
    if (result != 0) return MyString("__RUNTIME_ERROR__");

    // Read output file
    std::ifstream ifs(outputFile.c_str());
    if (!ifs.is_open()) return MyString("__RUNTIME_ERROR__");

    MyString output;
    char buf[1024];
    while (ifs.getline(buf, sizeof(buf))) {
        if (!output.empty()) output += "\n";
        output += buf;
    }
    ifs.close();
    return output;
}

bool EvaluationEngine::compareOutput(const MyString& actual, const MyString& expected) const {
    // Trim both and compare
    MyString a = actual.trim();
    MyString e = expected.trim();
    return a == e;
}

void EvaluationEngine::cleanup(const MyString& prefix) const {
    MyString cmd = MyString("rm -f ") + prefix + "*";
    system(cmd.c_str());
}

void EvaluationEngine::evaluate(IEvaluable* submission, Problem* problem) {
    if (!submission || !problem) {
        if (submission) submission->setVerdict(Verdict::RUNTIME_ERROR);
        return;
    }

    MyString sourceCode = submission->getSourceCode();
    MyString codeTemplate = problem->getCodeTemplate();

    // Wrap user code in template if template exists
    MyString fullCode;
    if (!codeTemplate.empty()) {
        fullCode = codeTemplate;
        fullCode.replace(MyString("{{USER_CODE}}"), sourceCode);
    } else {
        fullCode = sourceCode;
    }

    // Write source to temp file
    MyString sourceFile = tempDir + "/solution.cpp";
    MyString binaryFile = tempDir + "/solution";

    {
        std::ofstream ofs(sourceFile.c_str());
        if (!ofs.is_open()) {
            submission->setVerdict(Verdict::RUNTIME_ERROR);
            return;
        }
        ofs << fullCode;
        ofs.close();
    }

    // Compile
    if (!compileCode(sourceFile, binaryFile)) {
        submission->setVerdict(Verdict::COMPILATION_ERROR);
        cleanup(tempDir + "/solution");
        return;
    }

    // Run against each test case
    const DynamicArray<TestCase>& testCases = problem->getTestCases();
    int passed = 0;
    int total = testCases.size();

    for (int i = 0; i < total; i++) {
        // Write input to temp file
        MyString inputFile = tempDir + "/input.txt";
        {
            std::ofstream ofs(inputFile.c_str());
            ofs << testCases[i].getInput();
            ofs.close();
        }

        // Run
        MyString output = runBinary(binaryFile, inputFile);

        if (output == "__RUNTIME_ERROR__") {
            // Check if it was a timeout
            Submission* sub = dynamic_cast<Submission*>(submission);
            if (sub) sub->setTestResults(passed, total);
            submission->setVerdict(Verdict::RUNTIME_ERROR);
            cleanup(tempDir + "/");
            return;
        }

        // Compare
        if (compareOutput(output, testCases[i].getExpectedOutput())) {
            passed++;
        }
    }

    // Set results
    Submission* sub = dynamic_cast<Submission*>(submission);
    if (sub) sub->setTestResults(passed, total);

    if (passed == total) {
        submission->setVerdict(Verdict::ACCEPTED);
    } else {
        submission->setVerdict(Verdict::WRONG_ANSWER);
    }

    // Cleanup temp files immediately after evaluation
    cleanup(tempDir + "/");
}

MyString EvaluationEngine::getLastError() const {
    return MyString("");
}
