cmake_minimum_required(VERSION 3.20)

# Setting the VERSION on root project() will populate CMAKE_PROJECT_VERSION
# see: https://cmake.org/cmake/help/v3.13/variable/CMAKE_PROJECT_VERSION.html
project(stutter_win32_minimal)

add_executable(${PROJECT_NAME} WIN32 main.cpp)

find_package(OpenGL)

target_link_libraries(${PROJECT_NAME} OpenGL::GL)

