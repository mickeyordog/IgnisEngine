add_library(physics
    physicsContext.cpp
    physicsDebugRenderer.cpp
)

target_link_libraries(
    physics
    PUBLIC
        reactphysics3d
        OpenGL::GL
        GLEW::GLEW
)

target_include_directories(
    physics
    PUBLIC 
        reactphysics3d
        ../engine
)