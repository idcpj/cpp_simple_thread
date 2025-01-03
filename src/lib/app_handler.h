#pragma once
#include <string>
#include "connection_pool.h"

class AppHandler {
private:
    ConnectionPool dbPool;

public:
    AppHandler(int dbCount);
    void handleRequest(const std::string& request);
    ~AppHandler() = default;

private:
    std::string parseProtocol(const std::string& request);
    void queryDatabase(const std::string& query);
}; 