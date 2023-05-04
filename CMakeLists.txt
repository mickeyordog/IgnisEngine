cmake_minimum_required(VERSION 3.26.3)
# set (CMAKE_CXX_STANDARD 11)

project(IgnisEngine VERSION 0.1
                    DESCRIPTION "Game engine"
                    LANGUAGES C)
add_executable(${PROJECT_NAME} src/monoTest.c)

# clang src/monoTest.c -I libraries/MacOS/Mono.framework/Headers/mono-2.0 -L libraries/MacOS/Mono.framework/Libraries -lmono-2.0 
find_package(PkgConfig REQUIRED)
set(ENV{PKG_CONFIG_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/libraries/MacOS/Mono.framework/Versions/Current/lib/pkgconfig")
pkg_check_modules(MONO REQUIRED mono-2)
target_include_directories(${PROJECT_NAME} PRIVATE ${MONO_INCLUDE_DIRS})
target_link_directories(${PROJECT_NAME} PRIVATE ${MONO_LIBRARY_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${MONO_LIBRARIES})

target_compile_options(${PROJECT_NAME} PRIVATE ${MONO_CFLAGS})
target_link_options(${PROJECT_NAME} PRIVATE ${MONO_STATIC_LDFLAGS})

include(CMakePrintHelpers)
cmake_print_variables(MONO_INCLUDE_DIRS MONO_LIBRARY_DIRS MONO_LIBRARIES MONO_CFLAGS MONO_STATIC_LDFLAGS)
