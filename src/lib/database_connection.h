#pragma once
#include <string>
#include <iostream>

class DatabaseConnection {
public:
    void execute(const std::string& query);
}; 