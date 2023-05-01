add_library(util
    timer.cpp
)

target_link_libraries(
    util
    PUBLIC
        SDL2::SDL2
        SDL2_image::SDL2_image
)