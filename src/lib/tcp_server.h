#pragma once

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>
#endif

#include <iostream>
#include "thread_pool.h"
#include "app_handler.h"



class TCPServer {
private:
#ifdef _WIN32
    SOCKET server_fd;
#else
    int server_fd;
#endif
    struct sockaddr_in address;
    ThreadPool threadPool;
    AppHandler& appHandler;

public:
    TCPServer(int threadCount, AppHandler& appHandler, int port = 83);
    void start();
    ~TCPServer();

private:
    void handleClient(int clientSocket);
    void handleTCPConnection(AppHandler& app, const std::string& request);
    void setSocketOptions();
    void cleanup();
}; 