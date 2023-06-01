add_library(engine
    component.cpp
    objectTransform.cpp
    vec3.cpp
    scene.cpp
    gameObject.cpp
    transformationMatrix.cpp
    camera.cpp
)

target_link_libraries(
    engine
    PUBLIC
        SDL2::SDL2
        # OpenGL::GL
        # GLEW::glew_s
        STB_IMAGE
        IMGUI
        # input
)

target_include_directories(
    engine
    PUBLIC 
        ../input # need it like this bc it's relative to current. To fix could take reddit advice and move all into root
        ../visual
        ${STB_IMAGE_PATH}
        ${IMGUI_PATH}
)