add_library(visual
    SDLContext.cpp
    GLContext.cpp
    dearImGuiContext.cpp
)

target_link_libraries(
    visual
    PUBLIC
        SDL2::SDL2
        OpenGL::GL
        GLEW::GLEW
        STB_IMAGE
        IMGUI
        glm::glm
        engine
)

# get_target_property(GLM_INCLUDE_DIR glm::glm INTERFACE_INCLUDE_DIRECTORIES)

target_include_directories(
    visual
    PUBLIC 
        ../input # need it like this bc it's relative to current. To fix could take reddit advice and move all into root
        ../engine
        ${STB_IMAGE_PATH}
        ${IMGUI_PATH}
        # ${GLM_INCLUDE_DIR}
)

set(visual_files
    SDLContext.cpp
    GLContext.cpp
    dearImGuiContext.cpp
)