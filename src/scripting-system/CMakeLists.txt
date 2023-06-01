add_library(scripting-system
    pythonEngine.cpp
)

target_link_libraries(
    scripting-system
    PUBLIC
        SDL2::SDL2
        PRIVATE Python3::Python
        # visual
)