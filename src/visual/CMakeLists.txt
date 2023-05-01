add_library(visual
    window.cpp
    sprite.cpp
    gameObject.cpp
)

target_include_directories(
    visual
    PUBLIC 
        ${SDL2_INCLUDE_DIRS}
)
target_link_libraries(
    visual
    PUBLIC
        ${SDL2_LIBRARIES}
        SDL2_image::SDL2_image
)