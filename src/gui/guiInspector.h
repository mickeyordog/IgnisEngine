#pragma once

#include <vector>
#include <imgui.h>
#include "serialization.h"
#include "gameObject.h"

// TODO: component tab should be open by default
void showGuiInspectorPanel(const std::unordered_set<GameObject*>& selectedObjects) {
    ImGui::Begin("Inspector");

    if (selectedObjects.size() == 0)
    {
        ImGui::Text("No object selected");
    }
    else if (selectedObjects.size() == 1) {
        GameObject* const& gameObject = *selectedObjects.begin();
        if (gameObject->getComponents().size() == 0) {
            ImGui::Text("Object has no components");
            ImGui::End();
            return;
        }

        for (Component *component : (*selectedObjects.begin())->getComponents())
        {
            // ImGui::Text("New Component:"); // TODO: component name, maybe FieldDescription meta info type
            if (!ImGui::TreeNode("New Component"))
                continue;
                
            for (FieldDescription& f : component->getFields())
            {
                // ImGui::Text("Field name: %s", f.name);
                // ImGui::Text("Type enum: %d", (int)f.type);
                switch (f.type)
                {
                case FieldType::Int:
                    ImGui::Text("Value: %d", *(int*)f.ptr);
                    ImGui::SliderInt(f.name, (int*)f.ptr, -10, 10);
                    break;
                case FieldType::Float:
                    ImGui::Text("Value: %f", *(float *)f.ptr);
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
                }
                
            }
            ImGui::TreePop();
        }
    }
    else {
        ImGui::Text("Multiple objects selected");
    }
    

    ImGui::End();
}