#pragma once

#include <unordered_set>
#include "guiHierarchy.h"
#include "guiInspector.h"
#include "guiFileSystem.h"
#include "guiSceneView.h"
#include "scene.h"

void runIgnisEngineGui(Scene& scene) {
    static std::unordered_set<GameObject*> selectedGameObjects;

    showGuiHierarchyPanel(scene, selectedGameObjects);

    showGuiInspectorPanel(selectedGameObjects);

    showGuiFileSystemPanel();

    showGuiScenePanel(scene);
}