#pragma once

#include <unordered_set>
#include "serialization.h"
#include "component.h"

class EngineGuiManager {
#pragma region Singleton
public:
    static EngineGuiManager& getInstance() {
        static EngineGuiManager instance;
        return instance;
    }
    EngineGuiManager(EngineGuiManager const&) = delete;
    void operator=(EngineGuiManager const&) = delete;
private:
    EngineGuiManager() {}
#pragma endregion

public:
    void registerComponentClass(ClassDescription classDescription) { componentClasses.insert(classDescription); }
    const std::unordered_set<ClassDescription>& getAllComponentClasses() { return componentClasses; }

private:
    std::unordered_set<ClassDescription> componentClasses;
};