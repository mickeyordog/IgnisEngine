#pragma once

#include <unordered_set>
#include "serialization.h"
#include "component.h"

// TODO: this can prob just be static instead of singleton?
// TODO: for game specific components made outside engine, can let user specify their project location somewhere, and then engine knows to look there for a cmakelists to compile with rest of engine. User can include a specific function implemented by user that will register custom components to add to engine and let user run them in game
class EngineGuiManager {
// #pragma region Singleton
// public:
//     static EngineGuiManager& getInstance() {
//         static EngineGuiManager instance;
//         return instance;
//     }
//     EngineGuiManager(EngineGuiManager const&) = delete;
//     void operator=(EngineGuiManager const&) = delete;
// private:
//     EngineGuiManager() {}
// #pragma endregion

public:
    // void registerComponentClass(ClassDescription classDescription) { componentClasses.insert(classDescription); }
    // const std::unordered_set<ClassDescription>& getAllComponentClasses() { return componentClasses; }

private:
    // std::unordered_set<ClassDescription> componentClasses;
};