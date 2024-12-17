add_library(physics
    joltContext.cpp
)

target_link_libraries(
    physics
    PUBLIC
        Jolt::Jolt
)

target_include_directories(
    physics
    PUBLIC 
        unofficial::joltphysics::Jolt
)