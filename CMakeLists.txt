cmake_minimum_required(VERSION 3.10)
project(bpp)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 CONFIG REQUIRED)

file(GLOB_RECURSE target_sources
    "src/*.cpp"
    "lib/*.cpp"
)

add_executable(bpp
    ${target_sources}
)

target_include_directories(bpp PUBLIC
    "include"
    "lib/imgui/include"
    "lib/ImGuiFileDialog"
    SDL2::SDL2
)

target_link_libraries(bpp PUBLIC
    SDL2::SDL2 SDL2::SDL2main
)