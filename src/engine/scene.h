#pragma once

#include <vector>
#include "gameObject.h"

// TODO: save to json by iterating over all go's and using FieldDescription
class Scene {
public:
    Scene();
    ~Scene();

    void addRootGameObject(GameObject& gameObject);
    const std::vector<GameObject*>& getRootGameObjects() const { return this->rootObjects; }
    
private:
    std::vector<GameObject*> rootObjects;
};