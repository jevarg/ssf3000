cmake_minimum_required(VERSION 3.25)
project(ssf3000
    DESCRIPTION "farming3000 Screensaver")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXECUTABLE_SUFFIX ".scr")

add_executable(${PROJECT_NAME} WIN32
        src/main.cpp
        src/main.h
        src/ss3000.rc
        src/bitmap.h
        src/bitmap.h
        src/F3000.cpp
        src/F3000.h)

target_link_libraries(${PROJECT_NAME}
        Scrnsave.lib
        Gdiplus.lib
        Comctl32.dll)
