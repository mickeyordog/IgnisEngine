#pragma once

#include <vector>
#include <stack>
#include "asset.h"
#include "gameObject.h"
#include "cameraComponent.h"
#include "lightComponent.h"

struct SceneIteratorInfo {
    GameObject* gameObject;
    int depth;
};

// This class creates a stack (ie uses heap memory) every time, might not be worth it
class SceneIterator {
public:
    SceneIterator(Scene& scene);
    GameObject* getNext(bool skipChildren = false);
    SceneIteratorInfo getNextInfo(bool skipChildren = false);

private:
    SceneIteratorInfo current;
    std::stack<SceneIteratorInfo> objectStack;
};

// TODO: maybe want to generate a collection of all assets the scene needs, as a loading optimization so we don't unload from one to another
// Otherwise, could just unload all assets, then iterate through new scene to load all new ones
// TODO: the way I'm using the scene is not really an asset since it's changing. There should be another class that
// deals with the stuff that changes I think, meanwhile the scene asset class should be unique per .scene file
class Scene : public Asset {
public:
    Scene();
    virtual ~Scene() override;
    virtual Asset* clone() override { return new Scene(*this); }
    virtual AssetType getAssetType() const override { return AssetType::SCENE; }

    void addRootGameObject(GameObject* gameObject);
    void startGameObjects();
    void updateGameObjects(float dt);
    void fixedUpdateGameObjects(float dt);
    const std::vector<GameObject*>& getRootGameObjects() const { return this->rootObjects; }
    SceneIterator getIterator() { return SceneIterator(*this); }

    CameraComponent* findCamera();
    void render();
    CameraComponent* mainCamera = nullptr;
    std::vector<LightComponent*> lights; // might want to do this a different way
    double frameAlpha = 0.0; // Represents how far between the previous and current frame we are
    bool started = false;

private:
    std::vector<GameObject*> rootObjects; // TODO: replace these all with unique_ptr's, same with transform's children so they'll delete automatically. Also need to replace all iterations over scene with SceneIterator, and just return normal ptr from it
};