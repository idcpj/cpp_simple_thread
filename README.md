# C++ 多线程网络服务器

这是一个基于 C++11 的多线程网络服务器框架，实现了线程池、数据库连接池和 TCP 服务器的基本功能。

用于学习了解如何在 C++ 中使用 多线程处理,并如何构架一个简单的架构

## 功能特点

- 跨平台支持（Windows/Linux）
- 线程池实现
- 数据库连接池
- TCP 服务器
- 异步请求处理
- 优雅的资源管理

## 项目结构

```
cpp_simple_thread/
├── src/
│   ├── lib/
│   │   ├── database_connection.h/cpp   # 数据库连接类
│   │   ├── connection_pool.h/cpp       # 数据库连接池
│   │   ├── thread_pool.h/cpp          # 线程池实现
│   │   ├── app_handler.h/cpp          # 应用层处理
│   │   ├── tcp_server.h/cpp           # TCP 服务器
│   │   └── wsa_initializer.h/cpp      # Windows 套接字初始化
│   └── main.cpp                       # 主程序入口
├── Makefile                           # 项目构建文件
└── README.md                          # 项目说明文档
```

## 编译要求

- C++11 兼容的编译器
- Windows 或 Linux 操作系统
- Make 工具

### Windows 特定要求
- MinGW 或 MSVC 编译器
- Clang 编译器
- Windows Socket API (WinSock2)
- MSYS2

### Linux 特定要求
- GCC 或 Clang 编译器
- POSIX 线程库

## 构建说明

1. 清理项目：
```bash
make clean
```

2. 编译项目：
```bash
make 
```

3. 运行服务器：
```bash
# Windows
server.exe

# Linux
./server
```

## 配置说明

在 `main.cpp` 中可以配置以下参数：

- `dbCount`：数据库连接池大小（默认：5）
- `threadCount`：线程池大小（默认：4）
- `port`：服务器监听端口（默认：83）

## 使用示例

```cpp
try {
    // 创建应用处理器和服务器实例
    AppHandler appHandler(dbCount);
    TCPServer tcpServer(threadCount, appHandler, port);
    
    // 启动服务器
    tcpServer.start();
} catch (const std::exception& e) {
    std::cerr << "服务器错误: " << e.what() << std::endl;
}
```

## 特性说明

### 线程池
- 自动管理工作线程
- 任务队列
- 优雅关闭

### 数据库连接池
- 连接复用
- 自动连接管理
- 线程安全

### TCP 服务器
- 异步请求处理
- 多客户端支持
- 自动资源清理

## 注意事项

1. Windows 平台需要确保正确初始化 WinSock
2. 服务器默认监听所有网络接口（INADDR_ANY）
3. 确保防火墙允许服务器端口访问
4. 在生产环境中建议适当调整线程池和连接池大小

## 许可证

MIT License
## 贡献指南

1. Fork 项目
2. 创建特性分支
3. 提交更改
4. 推送到分支
5. 创建 Pull Request

## 联系方式

如有问题或建议，请提交 Issue。 