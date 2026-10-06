cmake_minimum_required(VERSION 3.14)

project(NyaES)

file(GLOB SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
add_executable(${PROJECT_NAME} ${SOURCES})

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(GLFW3 3.4 REQUIRED)
target_link_libraries(${PROJECT_NAME} GLFW3)

find_package(OpenGL 4.3 REQUIRED)