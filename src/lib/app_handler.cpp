#include "app_handler.h"

AppHandler::AppHandler(int dbCount) : dbPool(dbCount) {
}

void AppHandler::handleRequest(const std::string& request) {
    std::string command = parseProtocol(request);

    if (command == "GET") {
        queryDatabase("SELECT * FROM table_name");
    }
    else if (command == "POST") {
        queryDatabase("INSERT INTO table_name VALUES (...)");
    }

    std::cout << "Request processed: " << command << std::endl;
}

std::string AppHandler::parseProtocol(const std::string& request) {
    if (request.find("GET") == 0)
        return "GET";
    if (request.find("POST") == 0)
        return "POST";
    return "UNKNOWN";
}

void AppHandler::queryDatabase(const std::string& query) {
    auto conn = dbPool.getConnection();
    conn->execute(query);
} 