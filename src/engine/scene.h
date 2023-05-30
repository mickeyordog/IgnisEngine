#pragma once

#include <vector>
#include "gameObject.h"

class Scene {
public:
    Scene();
    ~Scene();

    void addRootGameObject(GameObject& gameObject);
    std::vector<GameObject*>& getRootGameObjects();
private:
    std::vector<GameObject*> rootObjects;
};