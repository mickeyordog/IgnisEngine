cmake_minimum_required(VERSION 3.26.3)

project(IgnisEngine)

# TODO: first line is mac specific, second line should direct to windows folder if on windows
set(CMAKE_BUILD_RPATH ${CMAKE_CURRENT_SOURCE_DIR}/libraries/MacOS/)
# list(APPEND CMAKE_INSTALL_RPATH ${CMAKE_CURRENT_SOURCE_DIR}/libraries/MacOS/)
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libraries/MacOS/)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})