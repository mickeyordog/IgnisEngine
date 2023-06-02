#include <stack>
#include "cameraComponent.h"

CameraComponent::CameraComponent(int width, int height, bool orthographic) : outputTexture(width, height)
{
    if (orthographic)
        projectionMatrix = glm::ortho(-(float)width / 2, (float)width / 2, -(float)height / 2, (float)height / 2, 0.1f, 100.0f); // TODO: why doesn't this work
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

    std::stack<GameObject*> objectStack;
    for (auto gameObjectIt = scene.getRootGameObjects().rbegin(); gameObjectIt != scene.getRootGameObjects().rend(); ++gameObjectIt)
    {
        objectStack.push(*gameObjectIt);
    }
    while (objectStack.size() > 0)
    {
        GameObject* currentObject = objectStack.top();
        objectStack.pop();
        for (auto transformIt = currentObject->transform.getChildTransforms().rbegin(); transformIt != currentObject->transform.getChildTransforms().rend(); ++transformIt)
        {
            objectStack.push((*transformIt)->parentGameObject);
        }

        for (ComponentVisual* visualComponent : currentObject->getVisualComponents()) {
            Shader& shader = visualComponent->getShader();
            glm::mat4 model = currentObject->transform.getData();
            glm::mat4 view = this->parentGameObject->transform.getData();
            glm::mat4 projection = this->projectionMatrix;
            glm::mat4 mvp = projection * view * model;
            shader.setUniform("mvp", mvp); // this doesn't really need to be set for all objects every frame

            visualComponent->render();
        }
    }

    outputTexture.unbind();
}
