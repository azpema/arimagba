cmake_minimum_required(VERSION 3.10)
project(arimagba VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_C_FLAGS_DEBUG "-g")

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# set(CMAKE_C_FLAGS_INIT "-fsanitize=undefined")
# set(CMAKE_CXX_FLAGS_INIT "-fsanitize=undefined")

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.hpp")

# Include directories
include_directories(include)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image>=2.0.0)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

# find_program(CLANG_TIDY_EXE NAMES "clang-tidy")

# if(CLANG_TIDY_EXE)
#     set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
# endif()

# Create executable
add_executable(${PROJECT_NAME}
 ${SOURCES}
 ${HEADERS})
 
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} imgui)

add_library(imgui STATIC
    libs/imgui-1.92.1/imgui_draw.cpp
    libs/imgui-1.92.1/imgui_tables.cpp
    libs/imgui-1.92.1/imgui_widgets.cpp
    libs/imgui-1.92.1/imgui.cpp

    libs/imgui-1.92.1/backends/imgui_impl_sdl2.cpp
    libs/imgui-1.92.1/backends/imgui_impl_sdlrenderer2.cpp

    libs/imgui-1.92.1/misc/cpp/imgui_stdlib.cpp
)

target_include_directories(
    imgui 
    PUBLIC 
    libs/imgui-1.92.1/ 
    libs/imgui-1.92.1/backends/
    libs/imgui-1.92.1/misc/cpp/
)
