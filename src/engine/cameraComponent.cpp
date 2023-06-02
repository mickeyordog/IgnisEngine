#include "cameraComponent.h"

CameraComponent::CameraComponent(int width, int height, bool orthographic) : outputTexture(width, height)
{
    if (orthographic)
        projectionMatrix = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);
    else
        projectionMatrix = glm::ortho(-(float)width / 2, (float)width / 2, -(float)height / 2, (float)height / 2, 0.1f, 100.0f); // doesn't seem to work
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

    for (GameObject* gameObject : scene.getRootGameObjects())
    {

    }

    outputTexture.unbind();
}
