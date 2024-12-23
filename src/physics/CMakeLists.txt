add_library(physics
    joltContext.cpp
    physicsDebugRenderer.cpp
)

target_link_libraries(
    physics
    PUBLIC
        Jolt::Jolt
)

target_include_directories(
    physics
    PUBLIC 
        Jolt::Jolt
        ../engine
)