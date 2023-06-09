#include "serializationHelper.h"
#include <stack>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

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
    nlohmann::json rootJson;
    for (auto gameObjectIt = scene.getRootGameObjects().rbegin(); gameObjectIt != scene.getRootGameObjects().rend(); ++gameObjectIt)
    {
        objectStack.push(*gameObjectIt);
    }
    while (objectStack.size() > 0)
    {
        std::cout << "stack size: " << objectStack.size() << std::endl;

        GameObject* currentObject = objectStack.top();
        objectStack.pop();

        nlohmann::json objectJson;
        objectJson["name"] = currentObject->name;
        objectJson["isActive"] = currentObject->isActive;

        nlohmann::json childListJson;
        for (auto& child : currentObject->transform.getChildTransforms())
        {
            childListJson.push_back((long long)child);
        }
        objectJson["children"] = childListJson;

        rootJson[std::to_string(currentID++)] = objectJson;
        // rootJson[(long)currentObject] = objectJson;
        
        auto& transforms = currentObject->transform.getChildTransforms();
        for (auto transformIt = transforms.rbegin(); transformIt != transforms.rend(); ++transformIt)
        {
            objectStack.push((*transformIt)->parentGameObject);
        }
    }

    std::ofstream o("../assets/savedScene.scene");
    o << std::setw(4) << rootJson << std::endl;
}

