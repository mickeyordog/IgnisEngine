#pragma once

#include <unordered_map>
#include <vector>
#include <memory>
#include "serialization.h"
#include "component.h"

class SerializationHelper
{
public:
    void registerComponentClass(enum ComponentType componentType, std::function<Component* (void)> constructor) { componentTypeToConstructor[componentType] = constructor; }
    Component* getNewComponent(enum ComponentType componentType) { return componentTypeToConstructor[componentType](); }
    std::unique_ptr<std::vector<const char*>> getComponentTypeNames();
    static const char* componentTypeToString(enum ComponentType type);
    static enum ComponentType stringToComponentType(const char* name);
private:
    std::unordered_map<enum ComponentType, std::function<Component* (void)> > componentTypeToConstructor;
};