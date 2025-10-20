cmake_minimum_required(VERSION 3.10)
project(ember C)

set(CMAKE_C_STANDARD 11)

# sdl
add_subdirectory(src/external/sdl)
include_directories(src/external/sdl/include)

#opengl
find_package(OpenGL REQUIRED)

# glad
add_library(glad src/external/glad/src/gl.c)
target_include_directories(glad PUBLIC src/external/glad/include)

# cglm
add_subdirectory(src/external/cglm)
include_directories(src/external/cglm/include)


# executable
add_executable(ember main.c)

# cgltf 
target_include_directories(ember PUBLIC ${CMAKE_SOURCE_DIR}/src/external/cgltf)


# libs
target_link_libraries(ember SDL3-shared glad OpenGL::GL cglm m)

