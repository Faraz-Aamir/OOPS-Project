// main.cpp — Entry point for Online Judge
// Persists VFS state to disk on shutdown (Ctrl+C) and restores on startup
// Faraz Aamir   | 25I-2069 | OOP Project | Spring 2026
// Shawwal       | 25I-2182 |
// Farhan        | 25I-2073 |

#include "OnlineJudge.h"
#include <iostream>
#include <csignal>

static OnlineJudge* judgeInstance = nullptr;

void signalHandler(int signum) {
    (void)signum;
    std::cout << "\n  Shutting down Online Judge..." << std::endl;
    if (judgeInstance) {
        judgeInstance->shutdown(); // Save state to disk before exit
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
