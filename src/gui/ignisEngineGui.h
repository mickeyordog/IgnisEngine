#pragma once

#include <unordered_set>
#include "guiHierarchy.h"
#include "guiInspector.h"
#include "guiFileSystem.h"
#include "guiSceneView.h"
#include "scene.h"

void runIgnisEngineGui(Scene& scene, bool& inGameView) {
    static std::unordered_set<GameObject*> selectedGameObjects;
    if (inGameView)
        ImGui::BeginDisabled();

    showGuiHierarchyPanel(scene, selectedGameObjects);

    showGuiInspectorPanel(selectedGameObjects);

    showGuiFileSystemPanel();
    if (inGameView)
        ImGui::EndDisabled();

    showGuiScenePanel(scene, inGameView);
}