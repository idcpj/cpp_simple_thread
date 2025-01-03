#include "tcp_server.h"
#include <stdexcept>
#include <string>

TCPServer::TCPServer(int threadCount, AppHandler& appHandler, int port)
    : threadPool(threadCount)
    , appHandler(appHandler)
{
    // 创建 socket
#ifdef _WIN32
    server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_fd == INVALID_SOCKET) {
        throw std::runtime_error("Socket创建失败, 错误码: " + std::to_string(WSAGetLastError()));
    }
#else
    server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_fd < 0) {
        throw std::runtime_error("Socket创建失败, 错误码: " + std::to_string(errno));
    }
#endif

    try {
        // 设置 socket 选项
        setSocketOptions();

        // 设置地址
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        // 绑定地址
#ifdef _WIN32
        if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
            throw std::runtime_error("绑定失败, 错误码: " + std::to_string(WSAGetLastError()));
        }
#else
        if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
            throw std::runtime_error("绑定失败, 错误码: " + std::to_string(errno));
        }
#endif
    }
    catch (...) {
        cleanup();
        throw;
    }
}

void TCPServer::cleanup() {
#ifdef _WIN32
    if (server_fd != INVALID_SOCKET) {
        closesocket(server_fd);
        server_fd = INVALID_SOCKET;
    }
#else
    if (server_fd >= 0) {
        close(server_fd);
        server_fd = -1;
    }
#endif
}

void TCPServer::setSocketOptions() {
    // 允许地址重用
    int opt = 1;
#ifdef _WIN32
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt)) == SOCKET_ERROR) {
        throw std::runtime_error("设置 socket 选项失败, 错误码: " + std::to_string(WSAGetLastError()));
    }
#else
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        throw std::runtime_error("设置 socket 选项失败, 错误码: " + std::to_string(errno));
    }
#endif
}

void TCPServer::start() {
#ifdef _WIN32
    if (listen(server_fd, SOMAXCONN) == SOCKET_ERROR) {
        throw std::runtime_error("监听失败, 错误码: " + std::to_string(WSAGetLastError()));
    }
#else
    if (listen(server_fd, SOMAXCONN) < 0) {
        throw std::runtime_error("监听失败, 错误码: " + std::to_string(errno));
    }
#endif

    std::cout << "服务器启动,监听" << ntohs(address.sin_port) << "端口..." << std::endl;

    while (true) {
#ifdef _WIN32
        SOCKET new_socket;
#else
        int new_socket;
#endif
        struct sockaddr_in client_addr;
        int addrlen = sizeof(client_addr);

#ifdef _WIN32
        if ((new_socket = accept(server_fd, (struct sockaddr*)&client_addr, &addrlen)) == INVALID_SOCKET) {
#else
        if ((new_socket = accept(server_fd, (struct sockaddr*)&client_addr, (socklen_t*)&addrlen)) < 0) {
#endif
            std::cerr << "接受连接失败, 错误码: " << 
#ifdef _WIN32
                WSAGetLastError()
#else
                errno
#endif
                << std::endl;
            continue;
        }

        // 获取客户端IP地址
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
        std::cout << "新的连接: " << client_ip << ":" << ntohs(client_addr.sin_port) << std::endl;

        threadPool.enqueue([this, new_socket]() { handleClient(new_socket); });
    }
}

TCPServer::~TCPServer() {
    cleanup();
}

void TCPServer::handleClient(int clientSocket) {
    char buffer[1024] = {0};
#ifdef _WIN32
    int bytes_received = recv(clientSocket, buffer, 1024, 0);
    if (bytes_received == SOCKET_ERROR) {
        std::cerr << "接收数据失败, 错误码: " << WSAGetLastError() << std::endl;
    }
#else
    int bytes_received = recv(clientSocket, buffer, 1024, 0);
    if (bytes_received < 0) {
        std::cerr << "接收数据失败, 错误码: " << errno << std::endl;
    }
#endif
    
    if (bytes_received > 0) {
        std::string request(buffer, bytes_received);
        handleTCPConnection(appHandler, request);
    }

#ifdef _WIN32
    closesocket(clientSocket);
#else
    close(clientSocket);
#endif
}

void TCPServer::handleTCPConnection(AppHandler& app, const std::string& request) {
    app.handleRequest(request);
} 