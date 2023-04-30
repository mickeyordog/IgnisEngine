cmake_minimum_required(VERSION 3.26.3)

project(IgnisEngine)

# not positive why I need to do this, look for better solution in future
if (APPLE)
    set(CMAKE_BUILD_RPATH ${CMAKE_CURRENT_SOURCE_DIR}/libraries/MacOS/)
endif()

if (APPLE)
    set(LIB_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/libraries/MacOS/)
elseif (WIN32)
    set(LIB_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/libraries/Windows/)
endif()

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})