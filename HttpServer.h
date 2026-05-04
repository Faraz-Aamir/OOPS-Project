// HttpServer.h — Minimal HTTP server for HTML/CSS/JS UI
// Uses POSIX sockets — no external libraries
// Faraz Aamir | 25I-2069 | OOP Project | Spring 2026

#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include "MyString.h"
#include "DynamicArray.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

// Forward declaration
class OnlineJudge;

class HttpServer {
private:
    int serverSocket;
    int port;
    OnlineJudge* app;
    bool running;

    // Parse HTTP request
    struct HttpRequest {
        MyString method;   // GET, POST
        MyString path;     // /api/login, /index.html
        MyString body;     // POST body (JSON-like)
        MyString getParam(const MyString& key) const;
    };

    struct HttpResponse {
        int statusCode;
        MyString contentType;
        MyString body;
    };

    HttpRequest parseRequest(const char* raw, int len) const;
    void sendResponse(int clientSocket, const HttpResponse& response) const;

    // Route handlers
    HttpResponse handleRequest(const HttpRequest& req);
    HttpResponse serveStaticFile(const MyString& path) const;
    HttpResponse handleAPI(const HttpRequest& req);

public:
    HttpServer(int port, OnlineJudge* app);
    ~HttpServer();

    bool start();
    void handleOneRequest(); // Handle a single request (non-blocking for main loop)
    void stop();
    bool isRunning() const;
};

#endif
