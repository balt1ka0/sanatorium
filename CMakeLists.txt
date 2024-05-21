cmake_minimum_required(VERSION 3.27)
project(sanatorium)
set(CMAKE_CXX_STANDARD 20)

add_executable(sanatorium main.cpp
        custom_exceptions.h
        queries.cpp
        models/guest.cpp
        models/room.cpp
        models/service.cpp
        validation.cpp
)

target_link_libraries(sanatorium sqlite3)

target_include_directories(${PROJECT_NAME}
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
        ${PROJECT_NAME}/models
        )
