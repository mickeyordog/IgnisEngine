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
    // outputTexture->bind();
    glClearColor(1.0, 0.7, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Mat4 view = this->gameObject->transform->getMatrix().inverted();
    const Mat4& projection = this->projectionMatrix;

    auto sceneIterator = scene.getIterator();
    while (GameObject* currentObject = sceneIterator.getNext()) {
        if (!currentObject->isActive)
            continue;

        const Mat4& model = currentObject->transform->getMatrix();
        Mat4 mvp = projection * view * model;
        // TODO: it would be great to just be able to call gameObject.render() on each one, but need to get shader and set uniforms
        for (auto& component : currentObject->getComponents())
        {
            if (!component->isActive || !component->isVisual())
                continue;
            ComponentVisual* visualComponent = (ComponentVisual*)component.get();

            Shader& shader = visualComponent->getShader();
            shader.setUniform("mvp", mvp.getData());

            visualComponent->render();
        }
    }
        
    // outputTexture->unbind();
}

void CameraComponent::setProjectionMatrix()
{
    if (orthographic)
        // projectionMatrix = glm::ortho(-(float)width / 2, (float)width / 2, -(float)height / 2, (float)height / 2, 0.1f, 1000.0f);
        projectionMatrix = Mat4(glm::ortho(-0.5f, 0.5f, -0.5f, 0.5f, 0.1f, 1000.0f)); // TODO: find good way to make this work
    else
        projectionMatrix = Mat4(glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f));
}

void CameraComponent::updateOutputTexture()
{
    if (outputTexture != nullptr)
        delete outputTexture;
    outputTexture = new RenderTexture(width, height);
}
