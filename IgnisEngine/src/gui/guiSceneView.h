#pragma once

#include <algorithm>
#include <stdio.h>
#include <imgui.h>
#include "scene.h"

void showGuiScenePanel(Scene& scene, bool& inGameView)
{
    ImGui::Begin("SceneView");
    ImVec2 windowSize = ImGui::GetContentRegionAvail();
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
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    if (ImGui::ImageButton("SceneViewPanel", (ImTextureID)(intptr_t)renderTexture.texture, windowSize, { 0, 1 }, { 1, 0 })) {
        inGameView = true;
    }
    ImGui::PopStyleVar(2);

    if (inGameView) { // TODO: call sdlctx.captureMouse(true) here
        SDL_SetRelativeMouseMode(SDL_TRUE);
        ImGui::SetNextFrameWantCaptureMouse(false);
        ImGui::SetNextFrameWantCaptureKeyboard(false);
    }

    ImGui::End();
}