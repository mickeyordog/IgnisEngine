add_library(component
    component.cpp
    transform.cpp
)

target_link_libraries(
    visual
    PUBLIC
        SDL2::SDL2
        SDL2_image::SDL2_image
        SDL2_ttf::SDL2_ttf
        OpenGL::GL
        GLEW::glew_s
        STB_IMAGE
        IMGUI
        # input
)

target_include_directories(
    visual
    PUBLIC 
        ../input # need it like this bc it's relative to current. To fix could take reddit advice and move all into root
        ${STB_IMAGE_PATH}
        ${IMGUI_PATH}
)