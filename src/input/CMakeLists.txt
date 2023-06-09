add_library(input
    inputHandler.cpp
)

target_link_libraries(
    input
    PUBLIC
        SDL2::SDL2
        # visual
)

set(input_files
    inputHandler.h
)