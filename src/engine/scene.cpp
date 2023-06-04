#include <stack>
#include "scene.h"

Scene::Scene()
{
    
}

Scene::~Scene()
{
    // TODO: iterate over all objects, from root upwards, and delete them
    // All gameobjects will need to be dynamically allocated
}

void Scene::addRootGameObject(GameObject* gameObject)
{
    this->rootObjects.push_back(gameObject);
}

void Scene::startGameObjects()
{
    // TODO: repeating this code in updateGameObjects, guiHierarchy, and cameraComponent
    std::stack<GameObject*> objectStack;
    for (auto gameObjectIt = rootObjects.rbegin(); gameObjectIt != rootObjects.rend(); ++gameObjectIt)
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

        currentObject->start();
    }
}

void Scene::updateGameObjects(float dt)
{
    std::stack<GameObject*> objectStack;
    for (auto gameObjectIt = rootObjects.rbegin(); gameObjectIt != rootObjects.rend(); ++gameObjectIt)
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

        currentObject->update(dt);
    }
}
