#pragma once

#include <unordered_map>
#include <vector>
#include <memory>
#include "serialization.h"
#include "component.h"

// This could maybe be replaced with a singleton if I want more control over its lifetime later
class SerializationHelper
{
public:
    static void registerComponentClass(ComponentClassInfo componentClassInfo);
    static Component* getNewComponent(enum ComponentType componentType);
    static std::vector<const char*>& getComponentTypeNames() { return componentTypeNames; }

    static const char* componentTypeToString(enum ComponentType type);
    static enum ComponentType stringToComponentType(const char* name);

private:
    SerializationHelper() { }
    static std::vector<ComponentClassInfo> componentClassInfos;
    static std::vector<const char*> componentTypeNames;
};