add_library(scripting-system
    pythonEngine.cpp
)

target_link_libraries(
    input
    PUBLIC
        SDL2::SDL2
        SDL2_image::SDL2_image
        # visual
)