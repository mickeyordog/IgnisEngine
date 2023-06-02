#include "cameraComponent.h"

CameraComponent::CameraComponent(int width, int height, bool orthographic) : outputTexture(width, height)
{
    if (orthographic)
        projectionMatrix = glm::ortho(-(float)width / 2, (float)width / 2, -(float)height / 2, (float)height / 2, 0.1f, 100.0f); // doesn't seem to work
    else
        projectionMatrix = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);
}

CameraComponent::~CameraComponent()
{

}

void CameraComponent::start()
{

}

void CameraComponent::update(float dt)
{

}

void CameraComponent::renderScene(const Scene& scene)
{
    outputTexture.bind();
    glClearColor(1.0, 0.7, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (GameObject* gameObject : scene.getRootGameObjects())
    {
        for (ComponentVisual* visualComponent : gameObject->getVisualComponents()) {
            Shader& shader = visualComponent->getShader();
            shader.setUniform("projection", this->projectionMatrix); // this doesn't really need to be set for all objects every frame
            shader.setUniform("view", this->parentGameObject->transform.getData());
            shader.setUniform("model", gameObject->transform.getData());

            visualComponent->render();
        }
    }

    outputTexture.unbind();
}
