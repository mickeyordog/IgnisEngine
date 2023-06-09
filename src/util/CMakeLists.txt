add_library(util
    timer.cpp
)

target_link_libraries(
    util
    PUBLIC
        SDL2::SDL2
)

set(util_files
    timer.cpp
)