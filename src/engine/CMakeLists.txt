add_library(engine
    animationClip.cpp
    animatorComponent.cpp
    animationController.cpp
    assetManager.cpp
    component.cpp
    componentPy.cpp
    componentVisual.cpp
    firstPersonController.cpp
    transformComponent.cpp
    scene.cpp
    gameObject.cpp
    cameraComponent.cpp
    inputHandler.cpp
    lightComponent.cpp
    mesh.cpp
    meshRenderer.cpp
    model.cpp
    quat.cpp
    renderTexture.cpp
    rigidBodyComponent.cpp
    shader.cpp
    spriteRenderer.cpp
    serializationHelper.cpp
    texture.cpp
    vec3.cpp
)

# set(engine_files
#     animationClip.cpp
#     animatorComponent.cpp
#     animationController.cpp
#     assetManager.cpp
#     component.cpp
#     componentPy.cpp
#     componentVisual.cpp
#     objectTransform.cpp
#     vec3.cpp
#     scene.cpp
#     gameObject.cpp
#     cameraComponent.cpp
#     renderTexture.cpp
#     shader.cpp
#     spriteRenderer.cpp
#     serializationHelper.cpp
#     texture.cpp
# )

target_link_libraries(
    engine
    PUBLIC
        SDL2::SDL2
        OpenGL::GL
        GLEW::GLEW
        STB_IMAGE
        IMGUI
        glm::glm
        visual
        assimp::assimp
        reactphysics3d
)

target_include_directories(
    engine
    PUBLIC 
        ../input # need it like this bc it's relative to current. To fix could take reddit advice and move all into root
        ../visual
        ../scripting-system
        ../util
        ../physics
        ${STB_IMAGE_PATH}
        ${IMGUI_PATH}
        ${LIGHTMAPPER_PATH}
        ${TINYGLTF_PATH}
        assimp::assimp
)