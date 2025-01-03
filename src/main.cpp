#include "tcp_server.h"
#include "app_handler.h"
#ifdef _WIN32
#include "wsa_initializer.h"
#endif

int main()
{
    try
    {
#ifdef _WIN32

        WSAInitializer wsaInit; // 初始化 WSA
#endif
        // 数据库连接池大小
        int dbCount = 5;
        // 线程池大小
        int threadCount = 4;
        // 服务器监听端口
        int port = 83;

        AppHandler appHandler(dbCount);
        TCPServer tcpServer(threadCount, appHandler, port);
        tcpServer.start();
    }
    
    catch (const std::exception &e)
    {
        std::cerr << "服务器错误: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}