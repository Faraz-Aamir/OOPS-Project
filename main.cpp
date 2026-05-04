// main.cpp — Entry point for Online Judge
// Faraz Aamir | 25I-2069 | OOP Project | Spring 2026

#include "OnlineJudge.h"
#include <iostream>
#include <csignal>

static OnlineJudge* judgeInstance = nullptr;

void signalHandler(int signum) {
    std::cout << "\n  Shutting down Online Judge..." << std::endl;
    if (judgeInstance) {
        judgeInstance->getVFS()->printFinalState(std::cout);
    }
    exit(0);
}

int main() {
    signal(SIGINT, signalHandler);

    OnlineJudge judge;
    judgeInstance = &judge;

    judge.initialize();
    judge.run(8080);

    return 0;
}
