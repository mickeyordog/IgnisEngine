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
        for (auto& child : currentObject->transform.getChildTransforms())
        {
            childListJson.push_back(child->parentGameObject->fileID);
        }
        objectJson["children"] = childListJson;

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
        
        auto& transforms = currentObject->transform.getChildTransforms();
        for (auto transformIt = transforms.rbegin(); transformIt != transforms.rend(); ++transformIt)
        {
            objectStack.push((*transformIt)->parentGameObject);
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
        GameObject* gameObject = new GameObject((*jsonIt)["name"].dump().c_str(), (*jsonIt)["fileID"]);
        gameObject->isActive = (*jsonIt)["isActive"];
        fileIDToGameObjectMap[gameObject->fileID] = gameObject;
    }
    for (auto& [fileID, gameObject] : fileIDToGameObjectMap) {
        nlohmann::ordered_json objectJson = sceneJson[std::to_string(fileID)];
        nlohmann::ordered_json childrenJson = objectJson["children"];
        for (auto& childFileID : childrenJson) {
            gameObject->transform.addChildTransform(fileIDToGameObjectMap[childFileID]->transform);
        }
        for (auto& component : objectJson["components"]) {
            // get type of component, create new component of that type, iterate over getFields, pass in values
        }
    }

    return scene;
}

