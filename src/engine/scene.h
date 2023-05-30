#pragma once

#include <vector>
#include "gameObject.h"

// TODO: save to json by iterating over all go's and using FieldDescription
class Scene {
public:
    Scene();
    ~Scene();

    void addRootGameObject(GameObject& gameObject);
    std::vector<GameObject*>& getRootGameObjects();
private:
    std::vector<GameObject*> rootObjects;
};