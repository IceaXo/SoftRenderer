cmake_minimum_required(VERSION 3.10)

# 定义项目名称和使用的语言 (CXX 代表 C++)
project(SoftRenderer CXX)

# 强制使用 C++17 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(MSVC)
    add_compile_options(/utf-8)
endif()

# 告诉包工头，头文件(菜单)都在 include 文件夹里
include_directories(include)

# 收集你当前所有的源文件(后厨)
set(SOURCES
    src/main.cpp
    src/framebuffer.cpp
    src/rasterizer.cpp
)

# 凝结法相：将所有源码打包成 SoftRenderer.exe
add_executable(SoftRenderer ${SOURCES})

include(CTest)
if(BUILD_TESTING)
    add_executable(framebuffer_tests tests/framebuffer_tests.cpp src/framebuffer.cpp)
    add_test(NAME framebuffer_bounds COMMAND framebuffer_tests)
endif()
