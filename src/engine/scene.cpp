#include "scene.h"

Scene::Scene()
{
    
}

Scene::~Scene()
{
    
}

void Scene::addRootGameObject(GameObject& gameObject)
{
    this->rootObjects.push_back(&gameObject);
}
