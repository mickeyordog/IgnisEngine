add_library(util
    timer.cpp
)

target_link_libraries(
    util
    PUBLIC
        SDL2::SDL2
)