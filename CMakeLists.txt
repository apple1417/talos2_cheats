cmake_minimum_required(VERSION 3.24)

project(talos2_cheats VERSION 1.0)
add_library(talos2_cheats SHARED)

target_compile_features(talos2_cheats PUBLIC cxx_std_20)
set_target_properties(talos2_cheats PROPERTIES
    EXPORT_COMPILE_COMMANDS True
    COMPILE_WARNING_AS_ERROR True
)

if(MSVC)
    target_compile_options(talos2_cheats PRIVATE /W4)
else()
    target_compile_options(talos2_cheats PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_subdirectory(libs/minhook)

add_library(imgui OBJECT
    "libs/imgui/imgui_demo.cpp"
    "libs/imgui/imgui_draw.cpp"
    "libs/imgui/imgui_tables.cpp"
    "libs/imgui/imgui_widgets.cpp"
    "libs/imgui/imgui.cpp"
    "libs/imgui/backends/imgui_impl_dx11.cpp"
    "libs/imgui/backends/imgui_impl_dx12.cpp"
    "libs/imgui/backends/imgui_impl_win32.cpp"
)
target_include_directories(imgui PUBLIC "libs/imgui" "libs/imgui/backends")

add_library(kiero OBJECT "libs/kiero/kiero.cpp")
target_include_directories(kiero PUBLIC "libs/kiero")
target_link_libraries(kiero PUBLIC minhook)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "src/*.cpp" "src/*.h")
target_sources(talos2_cheats PRIVATE ${sources})

target_include_directories(talos2_cheats PUBLIC "src")
target_link_libraries(talos2_cheats PUBLIC
    minhook
    kiero
    imgui

    dxguid.lib
    d3d11.lib
    d3d12.lib
)

target_precompile_headers(talos2_cheats PUBLIC "src/pch.h")
