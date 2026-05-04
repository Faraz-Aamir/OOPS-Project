// HttpServer.cpp — Minimal HTTP server implementation
// Faraz Aamir | 25I-2069 | OOP Project | Spring 2026

#include "HttpServer.h"
#include "OnlineJudge.h"
#include <cstring>
#include <cstdio>
#include <fstream>
#include <fcntl.h>

// Parse a simple key=value from URL-encoded or JSON-like body
MyString HttpServer::HttpRequest::getParam(const MyString& key) const {
    // Look for "key":"value" or key=value patterns
    MyString searchKey = MyString("\"") + key + "\":\"";
    int pos = body.find(searchKey);
    if (pos != -1) {
        int start = pos + searchKey.length();
        int end = body.find("\"", start);
        if (end != -1) return body.substr(start, end - start);
    }
    // Try key=value (URL encoded)
    searchKey = key + "=";
    pos = body.find(searchKey);
    if (pos != -1) {
        int start = pos + searchKey.length();
        int end = body.find("&", start);
        if (end == -1) end = body.length();
        return body.substr(start, end - start);
    }
    return MyString("");
}

HttpServer::HttpServer(int port, OnlineJudge* app)
    : serverSocket(-1), port(port), app(app), running(false) {}

HttpServer::~HttpServer() { stop(); }

HttpServer::HttpRequest HttpServer::parseRequest(const char* raw, int len) const {
    HttpRequest req;
    MyString rawStr(raw);

    // Parse method and path from first line
    int spacePos = rawStr.find(' ');
    if (spacePos != -1) {
        req.method = rawStr.substr(0, spacePos);
        int pathEnd = rawStr.find(' ', spacePos + 1);
        if (pathEnd != -1) {
            req.path = rawStr.substr(spacePos + 1, pathEnd - spacePos - 1);
        }
    }

    // Parse body (after double newline)
    int bodyStart = rawStr.find("\r\n\r\n");
    if (bodyStart != -1) {
        req.body = rawStr.substr(bodyStart + 4);
    }

    return req;
}

void HttpServer::sendResponse(int clientSocket, const HttpResponse& response) const {
    MyString header;
    header += "HTTP/1.1 ";
    header += MyString::fromInt(response.statusCode);
    header += (response.statusCode == 200 ? " OK" : " Error");
    header += "\r\nContent-Type: ";
    header += response.contentType;
    header += "\r\nContent-Length: ";
    header += MyString::fromInt(response.body.length());
    header += "\r\nAccess-Control-Allow-Origin: *";
    header += "\r\nAccess-Control-Allow-Methods: GET, POST, OPTIONS";
    header += "\r\nAccess-Control-Allow-Headers: Content-Type";
    header += "\r\nConnection: close\r\n\r\n";

    send(clientSocket, header.c_str(), header.length(), 0);
    if (response.body.length() > 0) {
        send(clientSocket, response.body.c_str(), response.body.length(), 0);
    }
}

bool HttpServer::start() {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) return false;

    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        close(serverSocket);
        return false;
    }

    listen(serverSocket, 10);

    // Set non-blocking
    int flags = fcntl(serverSocket, F_GETFL, 0);
    fcntl(serverSocket, F_SETFL, flags | O_NONBLOCK);

    running = true;
    std::cout << "  HTTP Server started on http://localhost:" << port << std::endl;
    return true;
}

void HttpServer::handleOneRequest() {
    if (!running) return;

    struct sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);
    int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
    if (clientSocket < 0) return; // No pending connection

    char buffer[65536];
    memset(buffer, 0, sizeof(buffer));
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);

    if (bytesRead > 0) {
        HttpRequest req = parseRequest(buffer, bytesRead);

        // Handle CORS preflight
        if (req.method == "OPTIONS") {
            HttpResponse resp;
            resp.statusCode = 200;
            resp.contentType = "text/plain";
            resp.body = "";
            sendResponse(clientSocket, resp);
        } else {
            HttpResponse resp = handleRequest(req);
            sendResponse(clientSocket, resp);
        }
    }
    close(clientSocket);
}

HttpServer::HttpResponse HttpServer::handleRequest(const HttpRequest& req) {
    // API routes
    if (req.path.startsWith("/api/")) {
        return handleAPI(req);
    }

    // Static files
    MyString path = req.path;
    if (path == "/") path = "/index.html";
    return serveStaticFile(path);
}

HttpServer::HttpResponse HttpServer::serveStaticFile(const MyString& path) const {
    HttpResponse resp;
    // Construct file path relative to 'web/' directory
    MyString filePath = MyString("web") + path;

    std::ifstream ifs(filePath.c_str());
    if (!ifs.is_open()) {
        resp.statusCode = 404;
        resp.contentType = "text/html";
        resp.body = "<h1>404 Not Found</h1>";
        return resp;
    }

    // Read entire file
    MyString content;
    char buf[4096];
    while (ifs.getline(buf, sizeof(buf))) {
        if (!content.empty()) content += "\n";
        content += buf;
    }
    ifs.close();

    resp.statusCode = 200;
    // Determine content type
    if (path.endsWith(".html")) resp.contentType = "text/html";
    else if (path.endsWith(".css")) resp.contentType = "text/css";
    else if (path.endsWith(".js")) resp.contentType = "application/javascript";
    else resp.contentType = "text/plain";
    resp.body = content;
    return resp;
}

HttpServer::HttpResponse HttpServer::handleAPI(const HttpRequest& req) {
    HttpResponse resp;
    resp.contentType = "application/json";
    resp.statusCode = 200;

    // Delegate to OnlineJudge
    resp.body = app->handleAPIRequest(req.path, req.method, req.body);
    return resp;
}

void HttpServer::stop() {
    if (running) {
        close(serverSocket);
        running = false;
    }
}

bool HttpServer::isRunning() const { return running; }
