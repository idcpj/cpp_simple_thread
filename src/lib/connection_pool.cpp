#include "connection_pool.h"

ConnectionPool::ConnectionPool(size_t pool_size) {
    for (size_t i = 0; i < pool_size; ++i) {
        pool.push(std::make_shared<DatabaseConnection>());
    }
}

std::shared_ptr<DatabaseConnection> ConnectionPool::getConnection() {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [this] { return !pool.empty(); });

    std::shared_ptr<DatabaseConnection> conn = pool.front();
    pool.pop();
    return conn;
} 