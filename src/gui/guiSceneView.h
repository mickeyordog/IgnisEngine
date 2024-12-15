#pragma once

#include <imgui.h>
#include "renderTexture.h"

#include <stdio.h>

void showGuiScenePanel(RenderTexture& renderTexture)
{
    ImGui::Begin("Scene");
    ImGui::Image((ImTextureID)(intptr_t)renderTexture.texture, ImVec2(renderTexture.width / 4, renderTexture.height / 4), { 0,1 }, { 1,0 });

    // printf("%p\n", (void*)(uintptr_t)renderTexture.texture);

    ImGui::End();
}