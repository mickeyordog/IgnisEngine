#pragma once

#include <vector>
#include <unordered_set>
#include <imgui.h>
#include "serialization.h"
#include "serializationHelper.h"
#include "gameObject.h"
#include "assetManager.h"

void showComponent(Component* component) {
    // ImGui::Text("New Component:"); // TODO: component name, maybe FieldDescription meta info type
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (!ImGui::TreeNode((void*)component, "%s", SerializationHelper::componentTypeToString(component->getType())))
        return;

    for (FieldDescription& f : component->getFields())
    {
        switch (f.type)
        {
        case FieldType::INT_FIELD:
            if (ImGui::DragInt(f.name, (int*)f.ptr))
                f.postUpdateFunction();
            break;
        case FieldType::FLOAT_FIELD:
            if (ImGui::DragFloat(f.name, (float*)f.ptr))
                f.postUpdateFunction();
            break;
        case FieldType::BOOL_FIELD:
            if (ImGui::Checkbox(f.name, (bool*)f.ptr))
                f.postUpdateFunction();
            break;
        case FieldType::VEC3_FIELD:
            if (ImGui::DragFloat3(f.name, (float*)f.ptr))
                f.postUpdateFunction();
            break;
        case FieldType::ASSET_POINTER_FIELD:
            if (!ImGui::TreeNode(f.name, "Pointer to %s: %p", f.name, *(void**)f.ptr))
                continue;
            for (const auto& [guid, filepath] : AssetManager::getRegisteredAssetMetaFilepaths())
            {
                if (filepath.extension().string() != f.validFileExtension)
                    continue;
                if (ImGui::Selectable(filepath.c_str())) {
                    *(Asset**)f.ptr = (Asset*)AssetManager::loadOrGetAsset(guid);
                }
            }
            ImGui::TreePop();
            break;
        default:
            break;
        }

    }
    ImGui::TreePop();
}

// TODO: component tab should be open by default
void showGuiInspectorPanel(const std::unordered_set<GameObject*>& selectedObjects)
{
    ImGui::Begin("Inspector");

    if (selectedObjects.size() == 0)
    {
        ImGui::Text("No object selected");
    }
    else if (selectedObjects.size() == 1) {
        GameObject* gameObject = *selectedObjects.begin();

        for (auto& component : gameObject->getComponents())
        {
            showComponent(component.get());
        }

        // ImGui::Button("Add Component");
        auto componentTypeNames = SerializationHelper::getComponentTypeNames();
        for (auto& name : componentTypeNames)
        {
            if (ImGui::Selectable(name))
            {
                gameObject->addComponentOfType(SerializationHelper::stringToComponentType(name));
            }
        }
    }
    else {
        ImGui::Text("Multiple objects selected");
    }
    
    ImGui::End();
}