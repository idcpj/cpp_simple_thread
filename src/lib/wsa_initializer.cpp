#ifdef _WIN32
#include "wsa_initializer.h"

WSAInitializer::WSAInitializer() {
     WSADATA wsaData;
        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (result != 0) {
            throw std::runtime_error("WSAStartup failed with error: " + std::to_string(result));
        }
        // 检查 Winsock 版本
        if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
            WSACleanup();
            throw std::runtime_error("Could not find a usable version of Winsock.dll");
        }
}

WSAInitializer::~WSAInitializer() {
    WSACleanup();
}
#endif 