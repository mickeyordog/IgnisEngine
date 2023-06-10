#include <stack>
#include <iostream>
#include "cameraComponent.h"

CameraComponent::CameraComponent(int width, int height, bool orthographic) : width(width), height(height), orthographic(orthographic), outputTexture(width, height)
{
    fields.insert(fields.begin(), Component::getFields().begin(), Component::getFields().end());

    setProjectionMatrix();
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

    std::stack<GameObject*> objectStack;
    for (auto gameObjectIt = scene.getRootGameObjects().rbegin(); gameObjectIt != scene.getRootGameObjects().rend(); ++gameObjectIt)
    {
        objectStack.push(*gameObjectIt);
    }
    while (objectStack.size() > 0)
    {
        GameObject* currentObject = objectStack.top();
        objectStack.pop();
        if (!currentObject->isActive)
            continue;
        for (auto transformIt = currentObject->transform->getChildTransforms().rbegin(); transformIt != currentObject->transform->getChildTransforms().rend(); ++transformIt)
        {
            objectStack.push((*transformIt)->gameObject);
        }
        // TODO: it would be great to just be able to call gameObject.render() on each one, but need to get shader and set uniforms

        for (auto& component : currentObject->getComponents()) {
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

    outputTexture.unbind();
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
    outputTexture = RenderTexture(width, height);
}
