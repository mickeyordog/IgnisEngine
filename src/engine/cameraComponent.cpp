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

// TODO: remove render loop from camera
void CameraComponent::renderScene(Scene& scene)
{
    outputTexture->bind();
    glClearColor(1.0, 0.7, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Mat4 view = this->gameObject->transform->getMatrix().inverted();
    const Mat4& projection = this->projectionMatrix;

    auto sceneIterator = scene.getIterator();
    bool skipChildren = false;
    while (GameObject* currentObject = sceneIterator.getNext(skipChildren)) {
        if (!currentObject->transform->isActive) {
            skipChildren = true;
            continue;
        }
        skipChildren = false;

        const Mat4& model = currentObject->transform->getMatrix();
        Mat4 mvp = projection * view * model;
        // TODO: it would be great to just be able to call gameObject.render() on each one, but need to get shader and set uniforms
        for (auto& component : currentObject->getComponents())
        {
            if (!component->isActive || !component->isVisual())
                continue;
            ComponentVisual* visualComponent = (ComponentVisual*)component.get();

            Shader* shader = visualComponent->getShader();
            if (shader == nullptr)
                continue;
            shader->setUniform("mvp", mvp.getData());

            // This should be distinguishing between light type. Might need a sep list for each, maybe could add it in lightcomponent ctor
            for (int i = 0; i < scene.lights.size(); i++) {
                scene.lights[i]->setShaderUniforms(*shader, i);
            }
            // might move these, only needed for lit 3d shader
            shader->setUniform("model", model.getData());
            shader->setInt("numDirLights", 1);
            shader->setInt("numPointLights", 0);
            shader->setInt("numSpotLights", 0);
            shader->setFloat("shininess", 32.0f);
            shader->setVec3("ambient", Vec3(0.1f, 0.1f, 0.1f));
            shader->setVec3("viewPos", gameObject->transform->getPosition().getData());

            visualComponent->render();
        }
    }
        
    outputTexture->unbind();
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
