#include "serializationHelper.h"
#include <stack>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include "randomNumberGenerator.h"
#include "assetManager.h"
#include "gameObject.h"

std::vector<ComponentClassInfo> SerializationHelper::componentClassInfos;
std::vector<const char*> SerializationHelper::componentTypeNames;

void SerializationHelper::registerComponentClass(ComponentClassInfo componentClassInfo)
{
    componentClassInfos.push_back(componentClassInfo);
    componentTypeNames.push_back(componentClassInfo.name);
}


Component* SerializationHelper::getNewComponent(enum ComponentType componentType)
{
    for (auto& classInfo : componentClassInfos)
    {
        if (classInfo.type == componentType)
            return classInfo.constructor();
    }
    return nullptr;
}

const char* SerializationHelper::componentTypeToString(enum ComponentType type)
{
    for (auto& classInfo : componentClassInfos)
    {
        if (classInfo.type == type)
            return classInfo.name;
    }
    return nullptr;
}

enum ComponentType SerializationHelper::stringToComponentType(const char* name)
{
    for (auto& classInfo : componentClassInfos)
    {
        if (strcmp(classInfo.name, name) == 0)
            return classInfo.type;
    }
    return ComponentType::UNKNOWN;
}

void SerializationHelper::serializeScene(Scene& scene)
{
    std::stack<GameObject*> objectStack;
    int currentID = 0;
    nlohmann::ordered_json rootJson;
    for (auto gameObjectIt = scene.getRootGameObjects().rbegin(); gameObjectIt != scene.getRootGameObjects().rend(); ++gameObjectIt)
    {
        objectStack.push(*gameObjectIt);
    }
    while (objectStack.size() > 0)
    {
        GameObject* currentObject = objectStack.top();
        objectStack.pop();

        nlohmann::ordered_json objectJson;
        objectJson["name"] = currentObject->name;
        objectJson["fileID"] = currentObject->fileID;
        objectJson["isActive"] = currentObject->isActive;

        nlohmann::ordered_json childListJson;
        for (auto& child : currentObject->transform->getChildTransforms())
        {
            childListJson.push_back(child->gameObject->fileID);
        }
        objectJson["children"] = childListJson;

        if (currentObject->transform->parentTransform == nullptr)
            objectJson["parent"] = -1;
        else
            objectJson["parent"] = currentObject->transform->parentTransform->gameObject->fileID;

        nlohmann::ordered_json componentsJsonList;
        for (auto& component : currentObject->getComponents())
        {
            nlohmann::ordered_json componentJson;
            componentJson["fileID"] = component->fileID;
            componentJson["type"] = static_cast<int>(component->getType());
            componentJson["typeName"] = componentTypeToString(component->getType());
            for (const FieldDescription& desc : component->getFields()) {
                nlohmann::ordered_json fieldJson;
                fieldJson["type"] = static_cast<int>(desc.type);
                switch (desc.type) {
                    case FieldType::INT_FIELD:
                        fieldJson["value"] = *(int*)desc.ptr;
                        break;
                    case FieldType::LLONG_FIELD:
                        fieldJson["value"] = *(long long*)desc.ptr;
                        break;
                    case FieldType::FLOAT_FIELD:
                        fieldJson["value"] = *(float*)desc.ptr;
                        break;
                    case FieldType::BOOL_FIELD:
                        fieldJson["value"] = *(bool*)desc.ptr;
                        break;
                    case FieldType::STRING_FIELD:
                        fieldJson["value"] = *(const char**)desc.ptr;
                        break;
                    case FieldType::VEC3_FIELD:
                        fieldJson["value"] = nlohmann::json::array({((glm::vec3*)desc.ptr)->x, ((glm::vec3*)desc.ptr)->y, ((glm::vec3*)desc.ptr)->z});
                        break;
                    case FieldType::ASSET_POINTER_FIELD:
                        fieldJson["value"] = (*(Asset**)desc.ptr)->guid;
                        break;
                    default:
                        std::cout << "Unknown field type " << static_cast<int>(desc.type) << std::endl;
                        break;
                }
                componentJson[desc.name] = fieldJson;
            }
            componentsJsonList[std::to_string(component->fileID)] = componentJson;
        }
        objectJson["components"] = componentsJsonList;

        rootJson[std::to_string(currentObject->fileID)] = objectJson;
        
        auto& transforms = currentObject->transform->getChildTransforms();
        for (auto transformIt = transforms.rbegin(); transformIt != transforms.rend(); ++transformIt)
        {
            objectStack.push((*transformIt)->gameObject);
        }
    }

    std::ofstream o("../assets/savedScene.scene"); // where should this path come from?
    o << std::setw(4) << rootJson << std::endl;
}

Scene* SerializationHelper::deserializeScene(const nlohmann::ordered_json& sceneJson)
{
    Scene* scene = new Scene();
    std::map<FileID, GameObject*> fileIDToGameObjectMap;
    for (auto jsonIt = sceneJson.rbegin(); jsonIt != sceneJson.rend(); ++jsonIt)
    {
        GameObject* gameObject = new GameObject((*jsonIt)["name"].dump(), (*jsonIt)["fileID"]);
        gameObject->isActive = (*jsonIt)["isActive"];
        fileIDToGameObjectMap[gameObject->fileID] = gameObject;
    }
    for (auto& [fileID, gameObject] : fileIDToGameObjectMap) {
        nlohmann::ordered_json objectJson = sceneJson[std::to_string(fileID)];
        nlohmann::ordered_json childrenJson = objectJson["children"];
        for (auto& childFileID : childrenJson) {
            gameObject->transform->addChildTransform(fileIDToGameObjectMap[childFileID]->transform);
        }
        if (objectJson["parent"] == -1) 
            scene->addRootGameObject(gameObject);
        else
            fileIDToGameObjectMap[objectJson["parent"]]->transform->addChildTransform(gameObject->transform);
        for (auto& component : objectJson["components"]) {
            // get type of component, create new component of that type, iterate over getFields, pass in values
            Component* newComponent;
            if (component["type"] == static_cast<int>(ComponentType::TRANSFORM))
                newComponent = gameObject->transform;
            else
                newComponent = gameObject->addComponentOfType(static_cast<ComponentType>(component["type"]));
            for (const FieldDescription& field : newComponent->getFields()) {
                switch (field.type) {
                    case FieldType::INT_FIELD:
                        *(int*)field.ptr = component[field.name]["value"];
                        // field.postUpdateFunction(); // This is causing camera to not output properly for some reason
                        break;
                    case FieldType::LLONG_FIELD:
                        *(long long*)field.ptr = component[field.name]["value"];
                        // field.postUpdateFunction();
                        break;
                    case FieldType::FLOAT_FIELD:
                        *(float*)field.ptr = component[field.name]["value"];
                        // field.postUpdateFunction();
                        break;
                    case FieldType::BOOL_FIELD:
                        *(bool*)field.ptr = component[field.name]["value"];
                        // field.postUpdateFunction();
                        break;
                    case FieldType::STRING_FIELD:
                        // *(const char**)field.ptr = component[field.name]["value"];
                        // field.postUpdateFunction();
                        break;
                    case FieldType::VEC3_FIELD:
                        ((glm::vec3*)field.ptr)->x = component[field.name]["value"][0];
                        ((glm::vec3*)field.ptr)->y = component[field.name]["value"][1];
                        ((glm::vec3*)field.ptr)->z = component[field.name]["value"][2];
                        field.postUpdateFunction();
                        break;
                    case FieldType::ASSET_POINTER_FIELD:
                        *(Asset**)field.ptr = AssetManager::loadOrGetAsset(component[field.name]["value"]);
                        // field.postUpdateFunction();
                        break;
                    default:
                        std::cout << "Unknown field type " << static_cast<int>(field.type) << std::endl;
                        break;
                }
            }
        }
    }

    return scene;
}

