#pragma once

#include <imgui.h>
#include "renderTexture.h"

void showGuiScenePanel(RenderTexture& renderTexture)
{
    ImGui::Begin("Scene");
    ImGui::Image((void*)(uintptr_t)renderTexture.texture, ImVec2(renderTexture.width / 4, renderTexture.height / 4), {0,1}, {1,0});

    ImGui::End();
}