#pragma once
#ifdef _WIN32
#include <winsock2.h>
#include <stdexcept>
#include <string>

class WSAInitializer {
public:
    WSAInitializer();
    ~WSAInitializer(); 
};
#endif 