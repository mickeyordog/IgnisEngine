#pragma once

#include <unordered_map>
#include <vector>
#include <memory>
#include "serialization.h"
#include "component.h"

class SerializationHelper
{
public: // TODO: remove old version with 2 params for register, refactor getters to use new vector
    // void registerComponentClass(enum ComponentType componentType, std::function<Component* (void)> constructor) { componentTypeToConstructor[componentType] = constructor; }
    static void registerComponentClass(ComponentClassInfo componentClassInfo) { componentClassInfos.push_back(componentClassInfo); };
    static Component* getNewComponent(enum ComponentType componentType);
    static std::unique_ptr<std::vector<const char*>> getComponentTypeNames();
    static const char* componentTypeToString(enum ComponentType type);
    static enum ComponentType stringToComponentType(const char* name);

private:
    // std::unordered_map<enum ComponentType, std::function<Component*(void)>> componentTypeToConstructor;
    static std::vector<ComponentClassInfo> componentClassInfos;
};