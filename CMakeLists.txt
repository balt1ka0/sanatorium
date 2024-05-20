cmake_minimum_required(VERSION 3.27)
project(sanatorium)
set(CMAKE_CXX_STANDARD 20)

add_executable(sanatorium main.cpp
        custom_exceptions.h
        queries.cpp)

target_link_libraries(sanatorium sqlite3)
