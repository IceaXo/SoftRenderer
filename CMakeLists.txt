# 声明要求的最低 CMake 版本
cmake_minimum_required(VERSION 3.10)

# 宗门项目名称与使用的语言标准 (C++17 是目前大厂最稳的基石)
project(SoftRenderer CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 告诉编译器，去 include 文件夹里找头文件
include_directories(include)

# 收集所有干活的源文件
set(SOURCES
    src/main.cpp
    src/framebuffer.cpp
    src/rasterizer.cpp
)

# 终极法术：把上面这些文件，熔炼成一个叫 SoftRenderer 的可执行程序！
add_executable(SoftRenderer ${SOURCES})