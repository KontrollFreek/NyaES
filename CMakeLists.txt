cmake_minimum_required(VERSION 3.14)

project(NyaES)

file(GLOB SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
add_executable(${PROJECT_NAME} ${SOURCES})

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(BUILD_SHARED_LIBS no)

add_subdirectory(glfw)
target_link_libraries(${PROJECT_NAME} glfw)

set(IMGUI_STATIC yes)
add_subdirectory(cimgui)
target_link_libraries(${PROJECT_NAME} cimgui)
target_compile_definitions(cimgui PUBLIC CIMGUI_USE_OPENGL3)
target_compile_definitions(cimgui PUBLIC CIMGUI_USE_GLFW)
target_sources(${PROJECT_NAME} PUBLIC
    ./cimgui/imgui/backends/imgui_impl_opengl3.cpp
    ./cimgui/imgui/backends/imgui_impl_glfw.cpp
)

add_subdirectory(glew)
target_link_libraries(${PROJECT_NAME} libglew_static)