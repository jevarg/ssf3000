cmake_minimum_required(VERSION 3.25)
project(ss3000
    DESCRIPTION "farming3000 Screensaver")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXECUTABLE_SUFFIX ".scr")

add_executable(${PROJECT_NAME} WIN32
        src/main.cpp
        src/main.h
        src/ss3000.rc
        src/bitmap.h
        src/bitmap.h)

target_link_libraries(${PROJECT_NAME}
        Scrnsave.lib
        Comctl32.dll)
