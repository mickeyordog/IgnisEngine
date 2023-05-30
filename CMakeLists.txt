# I just realized, why don't I just manually specify the locations of all
# my libraries? I know exactly where they are
# if you use a path like ${} you should always surround in quotes as it won't work if it has spaces and is unquoted
cmake_minimum_required(VERSION 3.26.3)
set (CMAKE_CXX_STANDARD 11)

project(IgnisEngine VERSION 0.1
                    DESCRIPTION "Game engine"
                    LANGUAGES CXX)
add_executable(${PROJECT_NAME} src/main.cpp)

# include_directories(${CMAKE_CURRENT_LIST_DIR}/src)

# not positive why I need to do this, look for better solution in future
# TODO: prob change these to {PROJECT_SOURCE_DIR}
if (APPLE)
    # TODO: how to specify path to framework when it's not in default location
    # outside the project??
    # set(CMAKE_MACOSX_RPATH 1)
    # set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
    # set(CMAKE_FRAMEWORK_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libraries/MacOS/)
    # set(LIB_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/libraries/MacOS/)
    # list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libraries/MacOS/)
    set (LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libraries/MacOS/)
elseif (WIN32)
    # Not sure why this is different on Windows
    # set(LIB_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/libraries/Windows/)
    list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libraries/Windows/)
endif()

# from reddit
# set(ENV{PKG_CONFIG_PATH} "${CMAKE_CURRENT_SOURCE_DIR}")
# set(PKGNAME Mono)
# include(FindPkgConfig)
# pkg_search_module(${PKGNAME} REQUIRED ${PKGNAME})
# mark_as_advanced(${PKGNAME}_FOUND ${PKGNAME}_CFLAGS ${PKGNAME}_STATIC_LDFLAGS)
# find_package_handle_standard_args(${PKGNAME} ${PKGNAME}_FOUND ${PKGNAME}_CFLAGS ${PKGNAME}_STATIC_LDFLAGS)

# list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}) # needed to find mono in root
# find_package(MONO REQUIRED) # this line I added, plus below
# target_compile_options(${PROJECT_NAME} PRIVATE ${${PKGNAME}_CFLAGS})
# target_link_options(${PROJECT_NAME} PRIVATE ${${PKGNAME}_STATIC_LDFLAGS})
# find_package(PkgConfig REQUIRED)
# set(ENV{PKG_CONFIG_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/libraries/MacOS/Mono.framework/Versions/Current/lib/pkgconfig")
# pkg_check_modules(MONO REQUIRED mono-2)
# target_include_directories(${PROJECT_NAME} PRIVATE ${MONO_INCLUDE_DIRS})
# target_link_directories(${PROJECT_NAME} PRIVATE ${MONO_LIBRARY_DIRS})
# target_link_libraries(${PROJECT_NAME} PRIVATE ${MONO_LIBRARIES})

# target_compile_options(${PROJECT_NAME} PRIVATE ${MONO_CFLAGS})
# target_link_options(${PROJECT_NAME} PRIVATE ${MONO_STATIC_LDFLAGS})
# include(CMakePrintHelpers)
# cmake_print_variables(MONO_INCLUDE_DIRS MONO_LIBRARY_DIRS MONO_LIBRARIES MONO_CFLAGS MONO_STATIC_LDFLAGS)
# target_compile_options(${PROJECT_NAME} PRIVATE "pkg-config --cflags mono-2")
# target_link_options(${PROJECT_NAME} PRIVATE "pkg-config --libs mono-2")

# target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libraries/MacOS/Mono.framework/Headers/mono-2.0)

# add_library(monoo ${CMAKE_CURRENT_SOURCE_DIR}/libraries/MacOS/Mono.framework/Libraries/mono)
# target_link_libraries(${PROJECT_NAME} PRIVATE monoo)
# set(CMAKE_FRAMEWORK_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libraries/MacOS/)
# target_link_libraries(${PROJECT_NAME} PRIVATE "-framework Mono")

# target_link_libraries(${PROJECT_NAME} PRIVATE "-framework SDL2")
find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)
find_package(SDL2_image CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>)
find_package(SDL2_ttf CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>)

find_package(Python3 COMPONENTS Development REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Python3::Python)

# find_package(SDL2 REQUIRED)
# find_package(SDL2_image REQUIRED)
# find_package(SDL2_ttf REQUIRED)
# find_package(MONO REQUIRED)

set (GLEW_DIR ${LIB_PATH}/glew/2.2.0_1/lib/cmake/glew)
find_package(GLEW REQUIRED)

find_package(OpenGL REQUIRED)

set(STB_IMAGE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libraries/cross-platform/stb_image)
add_library(STB_IMAGE ${STB_IMAGE_PATH}/stb_image.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${STB_IMAGE_PATH})
target_link_libraries(${PROJECT_NAME} PUBLIC STB_IMAGE)

set(IMGUI_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libraries/cross-platform/dear-imgui)
add_library(IMGUI ${IMGUI_PATH}/imgui.cpp ${IMGUI_PATH}/imgui_demo.cpp ${IMGUI_PATH}/imgui_draw.cpp ${IMGUI_PATH}/imgui_widgets.cpp ${IMGUI_PATH}/imgui_impl_sdl2.cpp ${IMGUI_PATH}/imgui_impl_opengl3.cpp ${IMGUI_PATH}/imgui_tables.cpp)
target_link_libraries(IMGUI PUBLIC SDL2::SDL2)
target_include_directories(${PROJECT_NAME} PUBLIC ${IMGUI_PATH})
target_link_libraries(${PROJECT_NAME} PUBLIC IMGUI)

set(SRC_DIRECTORIES input engine visual util audio scripting-system gui)
list(TRANSFORM SRC_DIRECTORIES PREPEND "src/" OUTPUT_VARIABLE SRC_PATHS)
foreach(DIR ${SRC_PATHS})
    add_subdirectory(${DIR})
endforeach()

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC 
        "${LIB_PATH}/glad/include"
        ${SRC_PATHS}
)
target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC
        OpenGL::GL
        GLEW::glew_s
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
