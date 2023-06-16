#pragma once

#include <vector>
#include <stack>
#include "asset.h"
#include "gameObject.h"
#include "cameraComponent.h"
// TODO: save to json by iterating over all go's and using FieldDescription
// TODO: maybe want to generate a collection of all assets the scene needs, as a loading optimization so we don't unload from one to another
// Otherwise, could just unload all assets, then iterate through new scene to load all new ones
class Scene : public Asset {
public:
    Scene();
    virtual ~Scene() override;
    virtual Asset* clone() override { return new Scene(*this); }

    void addRootGameObject(GameObject* gameObject);
    void startGameObjects();
    void updateGameObjects(float dt);
    // TODO: iterator for traversing entire scene tree
    const std::vector<GameObject*>& getRootGameObjects() const { return this->rootObjects; }
    CameraComponent* findCamera();
    void render();
    CameraComponent* mainCamera = nullptr;

private:
    std::vector<GameObject*> rootObjects; // TODO: replace these all with unique_ptr's, same with transform's children so they'll delete automatically. Also need to replace all iterations over scene with SceneIterator, and just return normal ptr from it

public:
    class SceneIterator {
    public:
        SceneIterator(Scene& scene) {
            for (auto gameObjectIt = scene.getRootGameObjects().rbegin(); gameObjectIt != scene.getRootGameObjects().rend(); ++gameObjectIt)
            {
                objectStack.push(*gameObjectIt);
            }
            current = objectStack.top();
        }

        GameObject* getNext(bool skipChildren = false) {
            if (current == nullptr)
                return nullptr;

            if (!skipChildren) {
                for (auto transformIt = current->transform->getChildTransforms().rbegin(); transformIt != current->transform->getChildTransforms().rend(); ++transformIt)
                {
                    objectStack.push((*transformIt)->gameObject);
                }
            }

            GameObject* ret = current;
            objectStack.pop();
            current = objectStack.top();
            return ret;
        }

    private:
        GameObject* current;
        std::stack<GameObject*> objectStack;
    };

public:
    SceneIterator getIterator() { return SceneIterator(*this); }
};