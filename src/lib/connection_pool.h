#pragma once
#include <queue>
#include <mutex>
#include <memory>
#include <condition_variable>
#include "database_connection.h"

class ConnectionPool {
public:
    ConnectionPool(size_t pool_size);
    std::shared_ptr<DatabaseConnection> getConnection();

private:
    std::queue<std::shared_ptr<DatabaseConnection>> pool;
    std::mutex mtx;
    std::condition_variable cv;
}; 