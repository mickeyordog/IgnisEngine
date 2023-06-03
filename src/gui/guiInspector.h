#pragma once

#include <vector>
#include <unordered_set>
#include <imgui.h>
#include "serialization.h"
#include "gameObject.h"

void showComponent(Component* component) {
    // ImGui::Text("New Component:"); // TODO: component name, maybe FieldDescription meta info type
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (!ImGui::TreeNode("New Component")) // TODO: these need to all have diff names, rn they're overlapping functionality
        return;

    for (FieldDescription& f : component->getFields())
    {
        switch (f.type)
        {
        case FieldType::Int:
            ImGui::Text("Value: %d", *(int*)f.ptr);
            ImGui::SliderInt(f.name, (int*)f.ptr, -10, 10);
            break;
        case FieldType::Float:
            ImGui::Text("Value: %f", *(float*)f.ptr); // would need to call update matrix here too, but then this slider would need to be specific to changing transform and non generic
            // ImGui::Text("Address: %p", f.ptr);
            ImGui::SliderFloat(f.name, (float*)f.ptr, -10.0, 10.0);
            break;
        case FieldType::Subclass:
            ImGui::BulletText("Subclass: %s", f.name);
            ImGui::TreePush(f.name); // TODO: this will still cause probs if multiple fields w/ same name
            break;
        case FieldType::EndSubclass:
            ImGui::TreePop(); // crashing bc subclass only pushes if interacted?
            // ImGui::TreeNode("End Subclass: %s", f.name);
            break;
        case FieldType::ComponentType:
            ImGui::Text("Component: %s", f.name);
            break;
        case FieldType::vec3:
            if (ImGui::SliderFloat3("Position", (float*)f.ptr, -10, 10))
                if (f.postUpdateFunction)
                    f.postUpdateFunction();
                // ((ObjectTransform*)f.objectPtr)->updateMatrix(); // why does this work without this??
        }

    }
    ImGui::TreePop();
}

// TODO: component tab should be open by default
void showGuiInspectorPanel(const std::unordered_set<GameObject*>& selectedObjects) {
    ImGui::Begin("Inspector");

    if (selectedObjects.size() == 0)
    {
        ImGui::Text("No object selected");
    }
    else if (selectedObjects.size() == 1) {
        GameObject* const& gameObject = *selectedObjects.begin();

        showComponent(&gameObject->transform);

        for (Component *component : gameObject->getComponents())
        {
            showComponent(component);
        }
        for (ComponentVisual* visualComponent : gameObject->getVisualComponents())
        {
            showComponent(visualComponent);
        }
    }
    else {
        ImGui::Text("Multiple objects selected");
    }
    
    ImGui::End();
}