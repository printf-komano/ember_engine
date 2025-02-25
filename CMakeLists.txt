cmake_minimum_required(VERSION 3.10)
project(ember C)

set(CMAKE_C_STANDARD 11)

# sdl
add_subdirectory(external/sdl)
include_directories(external/sdl/include)

#opengl
find_package(OpenGL REQUIRED)

# glad
add_library(glad external/glad/src/gl.c)
target_include_directories(glad PUBLIC external/glad/include)

# cglm
add_subdirectory(external/cglm)
include_directories(external/cglm/include)


# executable
add_executable(ember main.c)

# cgltf 
target_include_directories(ember PUBLIC ${CMAKE_SOURCE_DIR}/external/cgltf)


# libs
target_link_libraries(ember SDL3-shared glad OpenGL::GL cglm m)

