add_library(visual
    window.cpp
    sprite.cpp
    renderer.cpp
    gameObject.cpp
)

target_link_libraries(
    visual
    PUBLIC
        SDL2::SDL2
        SDL2_image::SDL2_image
        input
)

target_include_directories(
    visual
    PUBLIC 
        input
)