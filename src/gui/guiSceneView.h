#pragma once

#include <algorithm>
#include <stdio.h>
#include <imgui.h>
#include "scene.h"

void showGuiScenePanel(Scene& scene)
{
    ImGui::Begin("Scene");
    ImVec2 windowSize = ImGui::GetWindowSize();
    // windowSize.x = std::min(windowSize.x, windowSize.y);
    // windowSize.y = std::min(windowSize.x, windowSize.y);
    // When going fullscreen something about the viewport seems messed up for some reason
    // Seems like the camera is pointing in the wrong direction, need to move the cam to the right where it was 
    // before the resize to get the front-facing character to be looking at me
    if (scene.mainCamera->width != windowSize.x || scene.mainCamera->height != windowSize.y) {
        scene.mainCamera->width = windowSize.x;
        scene.mainCamera->height = windowSize.y;
        scene.mainCamera->setProjectionMatrix();
    }
    RenderTexture& renderTexture = scene.mainCamera->getOutputTexture();
    ImGui::ImageButton("ScenePanel", (ImTextureID)(intptr_t)renderTexture.texture, windowSize, { 0,1 }, { 1,0 });
    // how to get rid of button padding?

    bool isHovered = ImGui::IsItemHovered();
    bool isFocused = ImGui::IsItemFocused();
    ImVec2 mousePositionAbsolute = ImGui::GetMousePos();
    ImVec2 screenPositionAbsolute = ImGui::GetItemRectMin();
    ImVec2 mousePositionRelative = ImVec2(mousePositionAbsolute.x - screenPositionAbsolute.x, mousePositionAbsolute.y - screenPositionAbsolute.y);
    ImGui::Text("Is mouse over screen? %s", isHovered ? "Yes" : "No");
    ImGui::Text("Is screen focused? %s", isFocused ? "Yes" : "No");
    ImGui::Text("Position: %f, %f", mousePositionRelative.x, mousePositionRelative.y);
    ImGui::Text("Mouse clicked: %s", ImGui::IsMouseDown(ImGuiMouseButton_Left) ? "Yes" : "No");

    ImGui::End();
}