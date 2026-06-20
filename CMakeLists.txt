cmake_minimum_required(VERSION 3.27)
project(RetroFPSEngine LANGUAGES CXX)

set(UNITY_BUILD ON)

if (CMAKE_TOOLCHAIN_FILE)
    message(STATUS "Using VCPKG from: ${CMAKE_TOOLCHAIN_FILE}")
endif ()

find_package(SDL3 CONFIG REQUIRED)
find_package(wil CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(directxtk CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(stduuid CONFIG REQUIRED)

add_executable(RetroFPSEngine)
target_sources(RetroFPSEngine
        PRIVATE
        src/main.cpp
        src/Application.cpp
        src/Log.cpp
        src/platform/GameWindow.cpp
        src/platform/Input.cpp
        src/renderer/RenderDebug.cpp
        src/renderer/Framebuffer.cpp
        src/renderer/Renderer.cpp
        src/renderer/Adapter.cpp
        src/renderer/RenderContext.cpp
        src/renderer/ShaderHolder.cpp
        src/renderer/TextureHolder.cpp
        src/renderer/MeshHolder.cpp
        src/renderer/RenderGlobalBuffer.cpp
        src/scene/Scene.cpp
        src/game/Game.cpp
        src/imgui/imgui.cpp
        src/imgui/imgui_tables.cpp
        src/imgui/imgui_draw.cpp
        src/imgui/imgui_widgets.cpp
        src/imgui/imgui_demo.cpp
        src/imgui/imgui_impl_dx11.cpp
        src/imgui/imgui_impl_sdl3.cpp
        src/imgui/ToolRenderer.cpp

        PRIVATE
        FILE_SET HEADERS
        FILES
        src/Application.h
        src/Log.h
        src/platform/GameWindow.h
        src/platform/Input.h
        src/renderer/RenderDebug.h
        src/renderer/Framebuffer.h
        src/renderer/RendererCommon.h
        src/renderer/Renderer.h
        src/renderer/Adapter.h
        src/renderer/RenderContext.h
        src/renderer/ShaderHolder.h
        src/renderer/TextureHolder.h
        src/renderer/MeshHolder.h
        src/renderer/MeshBuilder.h
        src/renderer/MeshLoader.h
        src/renderer/RenderGlobalBuffer.h
        src/scene/Scene.h
        src/game/Game.h
        src/util/Mat4Ext.h
        src/util/StringExt.h
        src/util/MathExt.h
        src/Config.h
        src/util/RandomExt.h
        src/imgui/imconfig.h
        src/imgui/imgui_internal.h
        src/imgui/imgui.h
        src/imgui/imstb_rectpack.h
        src/imgui/imstb_textedit.h
        src/imgui/imstb_truetype.h
        src/imgui/imgui_impl_dx11.h
        src/imgui/imgui_impl_sdl3.h
        src/imgui/ToolRenderer.h
)
set_property(TARGET RetroFPSEngine PROPERTY COMPILE_WARNING_AS_ERROR ON)
target_compile_features(RetroFPSEngine PRIVATE cxx_std_23)
if (MSVC)
    target_compile_options(RetroFPSEngine PRIVATE /W4 /permissive- /EHsc)
    target_compile_definitions(RetroFPSEngine PRIVATE
            NOMINMAX
            WIN32_LEAN_AND_MEAN
    )
endif ()
target_link_libraries(RetroFPSEngine PRIVATE
        SDL3::SDL3
)
target_link_libraries(RetroFPSEngine PRIVATE
        d3d11
        dxgi
        d3dcompiler
        dxguid
        Microsoft::DirectXTK
        WIL::WIL
)
target_link_libraries(RetroFPSEngine PRIVATE spdlog::spdlog)
target_link_libraries(RetroFPSEngine PRIVATE assimp::assimp)
target_link_libraries(RetroFPSEngine PRIVATE stduuid)