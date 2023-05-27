add_library(scripting-system
    pythonEngine.cpp
)

target_link_libraries(
    scripting-system
    PUBLIC
        SDL2::SDL2
        SDL2_image::SDL2_image
        PRIVATE Python3::Python
        # visual
)