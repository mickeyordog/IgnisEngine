add_library(visual
    window.cpp
    sprite.cpp
    gameObject.cpp
)

target_link_libraries(
    visual
    PUBLIC
        SDL2::SDL2
        SDL2_image::SDL2_image
)