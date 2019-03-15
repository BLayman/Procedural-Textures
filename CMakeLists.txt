cmake_minimum_required(VERSION 2.8)
project (program3)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(../lib/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)

include_directories(../lib/glfw/include)
include_directories(../lib/glad/include)
include_directories(../lib/stb_image/include)
include_directories(../lib/glm)
include_directories(../lib/csci441/include)

set(LAB program3)
add_executable(${LAB} main.cpp ../lib/glad/src/glad.c ObjReader.h Vertex.h Shapes.h Texture.h FetchColor.h)
target_link_libraries(${LAB} glfw ${GLFW_LIBRARIES})

