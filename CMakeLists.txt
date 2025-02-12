cmake_minimum_required(VERSION 3.10)
project(ember C)

set(CMAKE_C_STANDARD 11)

# sdl
add_subdirectory(external/sdl)
include_directories(external/sdl/include)

# cglm
add_subdirectory(external/cglm)
include_directories(external/cglm/include)

# executable
add_executable(ember main.c)

# libs
target_link_libraries(ember SDL3-shared cglm)