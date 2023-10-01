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
        src/F3000.h
        src/gfx/dx/DXRenderer.cpp
        src/gfx/dx/DXRenderer.h
        src/gfx/dx/Shader.cpp
        src/gfx/dx/Shader.h
        src/gfx/dx/RenderTarget.cpp
        src/gfx/dx/RenderTarget.h
        src/gfx/dx/objects/Triangle.cpp
        src/gfx/dx/objects/Triangle.h
        src/gfx/dx/objects/Cube.cpp
        src/gfx/dx/objects/Cube.h
        src/gfx/dx/objects/AObject.h
        src/gfx/dx/DXConstantBuffer.h
        src/gfx/dx/ConstantBuffers.h)

target_link_libraries(${PROJECT_NAME}
        Scrnsave.lib
        Gdiplus.lib
        Comctl32.dll
        d3d11.lib
        dxgi.lib
        d3dcompiler.lib)
