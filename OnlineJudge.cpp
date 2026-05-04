// OnlineJudge.cpp — Main application controller implementation
// Faraz Aamir | 25I-2069 | OOP Project | Spring 2026

#include "OnlineJudge.h"
#include "HttpServer.h"

OnlineJudge::OnlineJudge() : vfs(nullptr), problemBank(nullptr),
    userManager(nullptr), sessionManager(nullptr), contestManager(nullptr) {}

OnlineJudge::~OnlineJudge() {
    delete contestManager;
    delete sessionManager;
    delete userManager;
    delete problemBank;
    delete vfs;
}

void OnlineJudge::initialize() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "  ONLINE JUDGE — Initializing..." << std::endl;
    std::cout << "========================================\n" << std::endl;

    // Create VFS
    vfs = new Volume("OnlineJudgeVFS");
    StorageMedium* ssd = vfs->addStorageMedium(100 * 1024 * 1024, MediumType::SSD); // 100 MB
    Partition* mainPartition = ssd->addPartition("main", 100 * 1024 * 1024, FileSystemFormat::EXT4,
        nullptr); // Will set owner after system user is created

    vfs->setPrimaryPartition(mainPartition);

    // Create system user (superuser)
    User* sysUser = vfs->addUser("system", 0, "/system", UserRole::SUPERUSER);
    mainPartition->getRootDirectory()->setOwner(sysUser);

    // Create base directories
    Directory* root = vfs->getRoot();
    root->createPath("system/problem_bank", sysUser);
    root->createPath("users", sysUser);
    root->createPath("config", sysUser);

    // Initialize components
    problemBank = new ProblemBank(vfs);
    problemBank->seedProblems();

    userManager = new UserManager(vfs);
    sessionManager = new SessionManager(vfs);
    contestManager = new ContestManager(vfs, problemBank);

    std::cout << "\n  Initialization complete!" << std::endl;
    std::cout << "  Problems loaded: " << problemBank->getTotalCount() << std::endl;
    std::cout << "  VFS ready.\n" << std::endl;
}

void OnlineJudge::run(int port) {
    HttpServer server(port, this);
    if (!server.start()) {
        std::cerr << "  ERROR: Failed to start HTTP server on port " << port << std::endl;
        return;
    }

    std::cout << "========================================" << std::endl;
    std::cout << "  ONLINE JUDGE is running!" << std::endl;
    std::cout << "  Open http://localhost:" << port << " in your browser" << std::endl;
    std::cout << "  Press Ctrl+C to stop" << std::endl;
    std::cout << "========================================\n" << std::endl;

    // Main event loop
    while (server.isRunning()) {
        server.handleOneRequest();
        usleep(1000); // 1ms sleep to avoid busy-waiting
    }
}

MyString OnlineJudge::jsonResponse(const MyString& status, const MyString& message, const MyString& data) const {
    MyString json = MyString("{\"status\":\"") + status + "\",\"message\":\"" + message + "\"";
    if (!data.empty()) {
        json += ",\"data\":" + data;
    }
    json += "}";
    return json;
}

MyString OnlineJudge::handleAPIRequest(const MyString& path, const MyString& method, const MyString& body) {
    // Helper to extract JSON fields
    auto getField = [&](const MyString& key) -> MyString {
        MyString searchKey = MyString("\"") + key + "\":\"";
        int pos = body.find(searchKey);
        if (pos == -1) return MyString("");
        int start = pos + searchKey.length();
        // Find closing quote, skipping escaped characters (e.g. \")
        int end = start;
        while (end < body.length()) {
            if (body[end] == '\\') {
                end += 2; // skip escaped character
                continue;
            }
            if (body[end] == '"') break;
            end++;
        }
        if (end >= body.length()) return MyString("");
        return body.substr(start, end - start);
    };

    // ===== USER ROUTES =====
    if (path == "/api/register" && method == "POST") {
        MyString username = getField("username");
        MyString email = getField("email");
        MyString password = getField("password");
        User* user = userManager->registerUser(username, email, password);
        if (user) return jsonResponse("ok", "Registration successful. Check your email for verification code.");
        return jsonResponse("error", "Registration failed. Username or email may already be taken.");
    }

    if (path == "/api/verify" && method == "POST") {
        MyString email = getField("email");
        MyString code = getField("code");
        if (userManager->verifyEmail(email, code))
            return jsonResponse("ok", "Email verified successfully!");
        return jsonResponse("error", "Invalid verification code.");
    }

    if (path == "/api/login" && method == "POST") {
        MyString username = getField("username");
        MyString password = getField("password");
        if (sessionManager->login(username, password)) {
            User* u = sessionManager->getCurrentUser();
            MyString userData = MyString("{\"username\":\"") + u->getUsername()
                + "\",\"email\":\"" + u->getEmail()
                + "\",\"contestGroup\":\"" + u->getContestGroup()
                + "\",\"score\":" + MyString::fromInt(u->getTotalScore())
                + ",\"verified\":" + (u->isVerified() ? "true" : "false") + "}";
            return jsonResponse("ok", "Login successful", userData);
        }
        return jsonResponse("error", "Login failed.");
    }

    if (path == "/api/logout" && method == "POST") {
        sessionManager->logout();
        return jsonResponse("ok", "Logged out.");
    }

    if (path == "/api/delete-account" && method == "POST") {
        if (!sessionManager->isLoggedIn()) return jsonResponse("error", "Not logged in.");
        MyString username = sessionManager->getCurrentUser()->getUsername();
        sessionManager->logout();
        userManager->deleteAccount(username);
        return jsonResponse("ok", "Account deleted.");
    }

    // ===== PROBLEM ROUTES =====
    if (path == "/api/problems" && method == "GET") {
        if (!sessionManager->isLoggedIn()) return jsonResponse("error", "Not logged in.");
        MyString data = "[";
        const DynamicArray<Problem*>& probs = problemBank->getAllProblems();
        for (int i = 0; i < probs.size(); i++) {
            if (i > 0) data += ",";
            data += "{\"id\":" + MyString::fromInt(probs[i]->getProblemId())
                + ",\"title\":\"" + probs[i]->getTitle()
                + "\",\"difficulty\":" + MyString::fromInt(probs[i]->getDifficulty()) + "}";
        }
        data += "]";
        return jsonResponse("ok", "Problems loaded", data);
    }

    if (path.startsWith("/api/problem/") && method == "GET") {
        if (!sessionManager->isLoggedIn()) return jsonResponse("error", "Not logged in.");
        MyString idStr = path.substr(13); // After "/api/problem/"
        int id = idStr.toInt();
        Problem* p = problemBank->getProblemById(id);
        if (!p) return jsonResponse("error", "Problem not found.");
        MyString data = "{\"id\":" + MyString::fromInt(p->getProblemId())
            + ",\"title\":\"" + p->getTitle()
            + "\",\"description\":\"" + p->getDescription()
            + "\",\"constraints\":\"" + p->getConstraints()
            + "\",\"difficulty\":" + MyString::fromInt(p->getDifficulty())
            + ",\"sampleInput\":\"" + p->getSampleInput()
            + "\",\"sampleOutput\":\"" + p->getSampleOutput() + "\"}";
        return jsonResponse("ok", "Problem loaded", data);
    }

    // ===== CONTEST ROUTES =====
    if (path == "/api/contest/create" && method == "POST") {
        if (!sessionManager->isLoggedIn()) return jsonResponse("error", "Not logged in.");
        User* user = sessionManager->getCurrentUser();
        Contest* existing = contestManager->getActiveContest(user->getUsername());
        if (existing) return jsonResponse("error", "You already have an active contest.");
        Contest* contest = contestManager->createContest(user);
        if (!contest) return jsonResponse("error", "Failed to create contest.");
        contestManager->startContest(contest);
        // Return contest info
        MyString data = "{\"contestId\":" + MyString::fromInt(contest->getContestId())
            + ",\"type\":\"" + contest->getContestType()
            + "\",\"profile\":\"" + contest->getDifficultyProfile()
            + "\",\"timeLimit\":" + MyString::fromInt(contest->getTimeLimit())
            + ",\"problemCount\":" + MyString::fromInt(contest->getProblemCount()) + "}";
        return jsonResponse("ok", "Contest started!", data);
    }

    if (path == "/api/contest/status" && method == "GET") {
        if (!sessionManager->isLoggedIn()) return jsonResponse("error", "Not logged in.");
        Contest* c = contestManager->getActiveContest(sessionManager->getCurrentUser()->getUsername());
        if (!c) return jsonResponse("ok", "No active contest", "null");
        MyString data = "{\"contestId\":" + MyString::fromInt(c->getContestId())
            + ",\"remaining\":" + MyString::fromInt(c->getRemainingTime())
            + ",\"score\":" + MyString::fromInt(c->getScore())
            + ",\"problemCount\":" + MyString::fromInt(c->getProblemCount())
            + ",\"submissions\":" + MyString::fromInt(c->getSubmissionCount()) + "}";
        return jsonResponse("ok", "Contest status", data);
    }

    if (path == "/api/contest/submit" && method == "POST") {
        if (!sessionManager->isLoggedIn()) return jsonResponse("error", "Not logged in.");
        User* user = sessionManager->getCurrentUser();
        Contest* c = contestManager->getActiveContest(user->getUsername());
        if (!c) return jsonResponse("error", "No active contest.");
        MyString indexStr = getField("problemIndex");
        MyString code = getField("code");
        // Unescape JSON escape sequences in code
        code.replace(MyString("\\\""), MyString("\""));
        code.replace(MyString("\\t"), MyString("\t"));
        code.replace(MyString("\\n"), MyString("\n"));
        code.replace(MyString("\\\\"), MyString("\\"));
        int idx = indexStr.toInt();
        Verdict v = contestManager->submitSolution(c, idx, code, user);
        MyString data = "{\"verdict\":\"" + IEvaluable::verdictToString(v) + "\"}";
        return jsonResponse("ok", "Submission evaluated", data);
    }

    if (path == "/api/contest/end" && method == "POST") {
        if (!sessionManager->isLoggedIn()) return jsonResponse("error", "Not logged in.");
        Contest* c = contestManager->getActiveContest(sessionManager->getCurrentUser()->getUsername());
        if (!c) return jsonResponse("error", "No active contest.");
        int score = contestManager->endContest(c);
        MyString data = "{\"score\":" + MyString::fromInt(score) + "}";
        return jsonResponse("ok", "Contest ended", data);
    }

    if (path == "/api/contest/save" && method == "POST") {
        if (!sessionManager->isLoggedIn()) return jsonResponse("error", "Not logged in.");
        // Find most recent completed contest
        return jsonResponse("ok", "Contest code saved.");
    }

    if (path == "/api/contest/discard" && method == "POST") {
        if (!sessionManager->isLoggedIn()) return jsonResponse("error", "Not logged in.");
        return jsonResponse("ok", "Contest code discarded.");
    }

    // ===== SAVE PROGRESS =====
    if (path == "/api/contest/save-progress" && method == "POST") {
        if (!sessionManager->isLoggedIn()) return jsonResponse("error", "Not logged in.");
        Contest* c = contestManager->getActiveContest(sessionManager->getCurrentUser()->getUsername());
        if (!c) return jsonResponse("error", "No active contest.");
        MyString indexStr = getField("problemIndex");
        MyString code = getField("code");
        code.replace(MyString("\\\""), MyString("\""));
        code.replace(MyString("\\t"), MyString("\t"));
        code.replace(MyString("\\n"), MyString("\n"));
        code.replace(MyString("\\\\"), MyString("\\"));
        contestManager->saveProgress(c, indexStr.toInt(), code);
        return jsonResponse("ok", "Progress saved.");
    }

    return jsonResponse("error", "Unknown API endpoint.");
}

Volume* OnlineJudge::getVFS() const { return vfs; }
SessionManager* OnlineJudge::getSessionManager() const { return sessionManager; }
ContestManager* OnlineJudge::getContestManager() const { return contestManager; }
