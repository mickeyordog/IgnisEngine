# I just realized, why don't I just manually specify the locations of all
# my libraries? I know exactly where they are
# if you use a path like ${} you should always surround in quotes as it won't work if it has spaces and is unquoted
cmake_minimum_required(VERSION 3.26.3)
set (CMAKE_CXX_STANDARD 11)

project(IgnisEngine VERSION 0.1
                    DESCRIPTION "Game engine"
                    LANGUAGES CXX)
add_executable(${PROJECT_NAME} src/main.cpp)

include_directories(${CMAKE_CURRENT_LIST_DIR}/src)

# not positive why I need to do this, look for better solution in future
# TODO: prob change these to {PROJECT_SOURCE_DIR}
if (APPLE)
    # TODO: how to specify path to framework when it's not in default location
    # outside the project??
    include(CMakePrintHelpers)
    cmake_print_variables(CMAKE_CURRENT_SOURCE_DIR)
    set(CMAKE_MACOSX_RPATH 1)
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
    set(CMAKE_FRAMEWORK_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libraries/MacOS/)
    set(LIB_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/libraries/MacOS/)
    list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libraries/MacOS/)
elseif (WIN32)
    # Not sure why this is different on Windows
    # set(LIB_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/libraries/Windows/)
    list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libraries/Windows/)
endif()

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

set(SRC_DIRECTORIES input visual util audio)
list(TRANSFORM SRC_DIRECTORIES PREPEND "src/" OUTPUT_VARIABLE SRC_PATHS)
foreach(DIR ${SRC_PATHS})
    add_subdirectory(${DIR})
endforeach()

target_include_directories(
    ${PROJECT_NAME} 
    PUBLIC 
        ${SDL2_INCLUDE_DIRS}
        # ${SDL2_IMAGE_INCLUDE_DIRS} TODO: why not needed?
        ${SRC_PATHS}
)
target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC
        SDL2::SDL2
        SDL2_image::SDL2_image # TODO: why can't this be ${SDL2_IMAGE_LIBRARIES}
        SDL2_ttf::SDL2_ttf
        ${SRC_DIRECTORIES}
)

if (WIN32)
    # from https://stackoverflow.com/questions/10671916/how-to-copy-dll-files-into-the-same-folder-as-the-executable-using-cmake
    # see if can find a better solution than this bc now I need to manually specify every dll
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD                                   # Adds a post-build event to MyTest
        COMMAND ${CMAKE_COMMAND} -E copy_if_different                                      # which executes "cmake - E copy_if_different..."
            "${CMAKE_CURRENT_SOURCE_DIR}/libraries/Windows/SDL2-2.26.5/lib/x64/SDL2.dll"   # <--this is in-file
            $<TARGET_FILE_DIR:${PROJECT_NAME}>)                                            # <--this is out-file path
endif()



# get_target_property(OUT ${PROJECT_NAME} INCLUDE_DIRECTORIES)
# message(STATUS ${OUT})
