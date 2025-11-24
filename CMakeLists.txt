cmake_minimum_required(VERSION 3.10)
project(FILESTAT CXX)
set(CMAKE_CXX_STANDARD 17)

# 添加UTF-8编译选项
if(MSVC)
    add_compile_options("/utf-8")
endif()

include_directories(include)
add_executable(file_stat main.cpp)