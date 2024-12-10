add_library(scripting_system
    pythonEngine.cpp
)

target_link_libraries(
    scripting_system
    PUBLIC
        SDL2::SDL2
        # PRIVATE Python3::Python
        # visual
)

set(scripting_system_files
    pythonEngine.cpp
)