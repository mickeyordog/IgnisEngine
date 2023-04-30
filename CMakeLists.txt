cmake_minimum_required(VERSION 3.26.3)

project(IgnisEngine)
set(SOURCE src/main.cpp)

set(SRC_DIRECTORIES window)
add_subdirectory(src/${SRC_DIRECTORIES})

# not positive why I need to do this, look for better solution in future
# TODO: prob change these to {PROJECT_SOURCE_DIR}
if (APPLE)
    set(CMAKE_BUILD_RPATH ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/MacOS/)
    set(LIB_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/MacOS/)
elseif (WIN32)
    # Not sure why this is different on Windows
    # set(LIB_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/libraries/Windows/)
    list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/Windows/)
endif()

add_executable(${PROJECT_NAME} ${SOURCE})

find_package(SDL2 REQUIRED)
target_include_directories(
    ${PROJECT_NAME} 
    PUBLIC 
        ${SDL2_INCLUDE_DIRS} 
        ${PROJECT_SOURCE_DIR}/src/window)
target_link_libraries(
    ${PROJECT_NAME} 
    PUBLIC 
        ${SDL2_LIBRARIES} 
        window
)

if (WIN32)
    # from https://stackoverflow.com/questions/10671916/how-to-copy-dll-files-into-the-same-folder-as-the-executable-using-cmake
    # see if can find a better solution than this bc now I need to manually specify every dll
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD                                   # Adds a post-build event to MyTest
        COMMAND ${CMAKE_COMMAND} -E copy_if_different                                      # which executes "cmake - E copy_if_different..."
            "${CMAKE_CURRENT_SOURCE_DIR}/Libraries/Windows/SDL2-2.26.5/lib/x64/SDL2.dll"   # <--this is in-file
            $<TARGET_FILE_DIR:${PROJECT_NAME}>)                                            # <--this is out-file path
endif()

include(CMakePrintHelpers)
cmake_print_variables(myLib)
