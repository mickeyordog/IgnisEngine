#include <stack>
#include "scene.h"

SceneIterator::SceneIterator(Scene& scene) {
    for (auto gameObjectIt = scene.getRootGameObjects().rbegin(); gameObjectIt != scene.getRootGameObjects().rend(); ++gameObjectIt)
    {
        objectStack.push(*gameObjectIt);
    }
    current = objectStack.top();
}

GameObject* SceneIterator::getNext(bool skipChildren)
{
    if (current == nullptr)
        return nullptr;

    GameObject* ret = current;
    objectStack.pop();

    if (!skipChildren)
    {
        for (auto transformIt = current->transform->getChildTransforms().rbegin(); transformIt != current->transform->getChildTransforms().rend(); ++transformIt)
        {
            objectStack.push((*transformIt)->gameObject);
        }
    }

    if (objectStack.size() > 0)
        current = objectStack.top();
    else
        current = nullptr;
    return ret;
}

Scene::Scene()
{
    
}

Scene::~Scene()
{
    SceneIterator it = getIterator();
    while (GameObject* gameObject = it.getNext())
    {
        delete gameObject; // TODO: this isn't working, need to instead implement option in iterator to do post order traversal, which will delete children first
    }
}

void Scene::addRootGameObject(GameObject* gameObject)
{
    this->rootObjects.push_back(gameObject);
}

void Scene::startGameObjects()
{
    SceneIterator it = getIterator();
    while (GameObject* gameObject = it.getNext())
    {
        gameObject->start();
    }
}

void Scene::updateGameObjects(float dt)
{
    SceneIterator it = getIterator();
    while (GameObject* gameObject = it.getNext())
    {
        gameObject->update(dt);
    }
}

CameraComponent* Scene::findCamera()
{
    SceneIterator it = getIterator();
    while (GameObject* gameObject = it.getNext())
    {
        if (Component* camComponent = gameObject->getComponentOfType(ComponentType::CAMERA); camComponent != nullptr)
            return (CameraComponent*)camComponent;
    }
    return nullptr;
}

void Scene::render()
{
    mainCamera->renderScene(*this);
}
