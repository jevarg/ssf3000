cmake_minimum_required(VERSION 3.25)
project(ssf3000
    DESCRIPTION "farming3000 Screensaver")

set(CMAKE_CXX_STANDARD 20)
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
        src/gfx/dx/FrameCB.h
        src/gfx/dx/ConstantBuffers.h
        src/gfx/ConstantBuffer.h
        src/gfx/dx/AppCB.h
        src/gfx/dx/objects/Head.cpp
        src/gfx/dx/objects/Head.h
        src/gfx/dx/objects/AObject.cpp)

set(TINYGLTF_HEADER_ONLY ON CACHE INTERNAL "" FORCE)
set(TINYGLTF_INSTALL OFF CACHE INTERNAL "" FORCE)
set(TINYGLTF_BUILD_LOADER_EXAMPLE OFF CACHE INTERNAL "" FORCE)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/lib/tinygltf)

target_link_libraries(${PROJECT_NAME}
        Scrnsave.lib
        Gdiplus.lib
        Comctl32.dll
        d3d11.lib
        dxgi.lib
        d3dcompiler.lib
        tinygltf)
