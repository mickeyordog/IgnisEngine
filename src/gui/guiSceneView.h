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
    ImGui::Image((ImTextureID)(intptr_t)renderTexture.texture, windowSize, { 0,1 }, { 1,0 });
    ImGui::End();
}