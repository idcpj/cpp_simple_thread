CXX = c++
CXXFLAGS = -std=c++11 -Wall -pthread -I./src/lib
LDFLAGS = 

# 源文件目录
SRC_DIR = src
LIB_DIR = src/lib

# 检测操作系统并设置相应的命令
ifeq ($(OS),Windows_NT)
    LDFLAGS += -lws2_32
    TARGET = server.exe
    # 检测是否在 Git Bash 或 WSL 环境
    ifneq (,$(findstring /,$(SHELL)))
        # Git Bash/WSL 环境使用 rm 命令
        MKDIR = mkdir -p
        RM = rm -f
        RMDIR = rm -rf
        SEP = /
    else
        # 原生 Windows 命令
        MKDIR = mkdir
        RM = del /F /Q
        RMDIR = rmdir /S /Q
        SEP = \\
    endif
else
    TARGET = server
    MKDIR = mkdir -p
    RM = rm -f
    RMDIR = rm -rf
    SEP = /
endif

# 源文件
SRCS = $(SRC_DIR)/main.cpp \
       $(LIB_DIR)/tcp_server.cpp \
       $(LIB_DIR)/app_handler.cpp \
       $(LIB_DIR)/thread_pool.cpp \
       $(LIB_DIR)/connection_pool.cpp \
       $(LIB_DIR)/database_connection.cpp

# Windows 下额外添加 wsa_initializer.cpp
ifeq ($(OS),Windows_NT)
    SRCS += $(LIB_DIR)/wsa_initializer.cpp
endif

# 创建输出目录
OBJDIR = obj

# 生成目标文件路径
OBJS = $(patsubst %.cpp,$(OBJDIR)/%.o,$(notdir $(SRCS)))

# 主要目标
all: $(OBJDIR) $(TARGET)

# 创建目标目录
$(OBJDIR):
	-$(MKDIR) $(OBJDIR)

# 链接目标文件
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

# 编译源文件
$(OBJDIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(LIB_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean all

# 清理目标
clean:
	-$(RM) $(OBJDIR)$(SEP)*.o $(TARGET)
	-$(RMDIR) $(OBJDIR)