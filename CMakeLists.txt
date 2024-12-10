cmake_minimum_required(VERSION 3.26.3)
set (CMAKE_CXX_STANDARD 17) # need 17 for imfilebrowser, but could otherwise use 14 for make_unique
add_compile_options("$<$<CONFIG:DEBUG>:-DDEBUG>")

project(IgnisEngine VERSION 0.1
                    DESCRIPTION "Game engine"
                    LANGUAGES CXX)
add_executable(${PROJECT_NAME} src/main.cpp)

# if (APPLE)
#     set (LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libraries/MacOS/)
# elseif (WIN32)
#     list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libraries/Windows/)
# endif()

if (WIN32)
    set(SDL2_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows-static/share/sdl2)
endif()
find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)

# got warning "WARNING: Static builds of Python will not have C extension modules available." on Windows. If Python doesn't work here that's why
# currently using my system python which doesnt have debug, instead of vendored 3.10
if (APPLE)
    set(Python3_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/arm64-osx/share/python3)
elseif (WIN32)
    set(Python3_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows/share/python3)
endif()
find_package(Python3 COMPONENTS Development REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Python3::Python)

if (WIN32)
    set(GLEW_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows-static/share/glew)
endif()
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE GLEW::GLEW OpenGL::GL)

set(STB_IMAGE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libraries/cross-platform/stb_image)
add_library(STB_IMAGE ${STB_IMAGE_PATH}/stb_image.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${STB_IMAGE_PATH})
target_link_libraries(${PROJECT_NAME} PUBLIC STB_IMAGE)

# TODO: prob remove this
set(LIGHTMAPPER_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libraries/cross-platform/lightmapper)
target_include_directories(${PROJECT_NAME} PUBLIC ${LIGHTMAPPER_PATH})

set(IMGUI_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libraries/cross-platform/dear-imgui)
add_library(IMGUI ${IMGUI_PATH}/imgui.cpp ${IMGUI_PATH}/imgui_demo.cpp ${IMGUI_PATH}/imgui_draw.cpp ${IMGUI_PATH}/imgui_widgets.cpp ${IMGUI_PATH}/imgui_impl_sdl2.cpp ${IMGUI_PATH}/imgui_impl_opengl3.cpp ${IMGUI_PATH}/imgui_tables.cpp)
target_link_libraries(IMGUI PUBLIC SDL2::SDL2)
target_include_directories(${PROJECT_NAME} PUBLIC ${IMGUI_PATH})
target_link_libraries(${PROJECT_NAME} PUBLIC IMGUI)

if (APPLE)
    set(nlohmann_json_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/arm64-osx/share/nlohmann_json)
elseif (WIN32)
    set(nlohmann_json_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows-static/share/nlohmann_json)
endif()
find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

if (APPLE)
    set(glm_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/arm64-osx/share/glm)
elseif (WIN32)
    set(glm_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows-static/share/glm)
endif()
find_package(glm CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)

include(FetchContent)
set(FETCHCONTENT_BASE_DIR ${PROJECT_SOURCE_DIR}/libs CACHE PATH "" FORCE)
# assimp
FetchContent_Declare(assimp
GIT_REPOSITORY https://github.com/assimp/assimp.git
GIT_TAG master)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(assimp)
target_include_directories(${PROJECT_NAME} PRIVATE libs/assimp-src/include)
target_link_libraries(${PROJECT_NAME} PRIVATE assimp)

set(SRC_DIRECTORIES engine visual util audio scripting_system gui)
list(TRANSFORM SRC_DIRECTORIES PREPEND "src/" OUTPUT_VARIABLE SRC_PATHS)
foreach(DIR ${SRC_PATHS})
    add_subdirectory(${DIR})
endforeach()

target_include_directories(${PROJECT_NAME} PUBLIC ${SRC_PATHS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${SRC_DIRECTORIES})

# get_target_property(OUT ${PROJECT_NAME} INCLUDE_DIRECTORIES)
# message(STATUS ${OUT})
