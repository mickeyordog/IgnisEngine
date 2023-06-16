#include <stack>
#include <iostream>
#include "cameraComponent.h"
#include "scene.h"

CameraComponent::CameraComponent(int width, int height, bool orthographic) : width(width), height(height), orthographic(orthographic)
{
    fields.insert(fields.begin(), Component::getFields().begin(), Component::getFields().end());

    updateOutputTexture();
    setProjectionMatrix();
}

CameraComponent::~CameraComponent()
{
    if (outputTexture != nullptr)
        delete outputTexture;
}

void CameraComponent::start()
{

}

void CameraComponent::update(float dt)
{

}

void CameraComponent::renderScene(Scene& scene)
{
    outputTexture->bind();
    glClearColor(1.0, 0.7, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto sceneIterator = scene.getIterator();
    while (GameObject* currentObject = sceneIterator.getNext()) {
        if (!currentObject->isActive)
            continue;

        // TODO: it would be great to just be able to call gameObject.render() on each one, but need to get shader and set uniforms
        for (auto& component : currentObject->getComponents())
        {
            if (!component->isActive || !component->isVisual())
                continue;
            ComponentVisual* visualComponent = (ComponentVisual*)component.get();

            Shader& shader = visualComponent->getShader();
            glm::mat4 model = currentObject->transform->getMatrix();
            // TODO: I don't think taking inverse to get view is correct
            glm::mat4 view = glm::inverse(this->gameObject->transform->getMatrix()); // Is compiler smart enough to only compute this once?
            glm::mat4 projection = this->projectionMatrix;
            glm::mat4 mvp = projection * view * model;
            shader.setUniform("mvp", mvp);

            visualComponent->render();
        }
    }
        
    outputTexture->unbind();
}

void CameraComponent::setProjectionMatrix()
{
    if (orthographic)
        // projectionMatrix = glm::ortho(-(float)width / 2, (float)width / 2, -(float)height / 2, (float)height / 2, 0.1f, 1000.0f);
        projectionMatrix = glm::ortho(-0.5f, 0.5f, -0.5f, 0.5f, 0.1f, 1000.0f); // TODO: find good way to make this work
    else
        projectionMatrix = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);
}

void CameraComponent::updateOutputTexture()
{
    // outputTexture.~RenderTexture();
    if (outputTexture != nullptr)
        delete outputTexture;
    outputTexture = new RenderTexture(width, height);
}
