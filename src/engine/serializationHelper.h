#pragma once

#include <unordered_map>
#include "serialization.h"

class SerializationHelper
{
public:
    void registerComponentClass(enum ComponentType componentType, std::function<Component* (void)> constructor) { componentTypeToConstructor[componentType] = constructor; }
    Component* getNewComponent(enum ComponentType componentType) { return componentTypeToConstructor[componentType](); }
private:
    std::unordered_map<enum ComponentType, std::function<Component* (void)> > componentTypeToConstructor;
};