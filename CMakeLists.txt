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

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "src/*.cpp" "src/*.h")
target_sources(talos2_cheats PRIVATE ${sources})

target_include_directories(talos2_cheats PUBLIC "src")
target_link_libraries(talos2_cheats PUBLIC minhook)

target_precompile_headers(talos2_cheats PUBLIC "src/pch.h")
