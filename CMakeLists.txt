cmake_minimum_required(VERSION 3.14)
project(overHead)
set(CMAKE_CXX_STANDARD 17)


find_package(GLFW3 REQUIRED)
add_executable(overHead main.cpp ./src/glad.c)

target_link_libraries(overHead ${GLFW_LIBRARIES} glfw)