cmake_minimum_required(VERSION 3.14)
project(overHead)
set(CMAKE_CXX_STANDARD 17)


find_package(GLFW3 REQUIRED)
set(SOURCE_FILES ./src/main.cpp ./src/glad.c ./src/function.cpp)
add_executable(overHead ${SOURCE_FILES})

target_link_libraries(overHead ${GLFW_LIBRARIES} glfw)