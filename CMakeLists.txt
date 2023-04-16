cmake_minimum_required(VERSION 3.26.3)

project(IgnisEngine)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})