#pragma once

#include <vector>
#include "asset.h"
#include "gameObject.h"
#include "cameraComponent.h"
// TODO: save to json by iterating over all go's and using FieldDescription
// TODO: maybe want to generate a collection of all assets the scene needs, as a loading optimization so we don't unload from one to another
// Otherwise, could just unload all assets, then iterate through new scene to load all new ones
class Scene : public Asset {
public:
    Scene();
    ~Scene();

    void addRootGameObject(GameObject* gameObject);
    void startGameObjects();
    void updateGameObjects(float dt);
    // TODO: iterator for traversing entire scene tree
    const std::vector<GameObject*>& getRootGameObjects() const { return this->rootObjects; }
    CameraComponent* findCamera();
    void render();
    CameraComponent* mainCamera = nullptr;

private:
    std::vector<GameObject*> rootObjects;
};