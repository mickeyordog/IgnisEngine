#pragma once

#include <vector>
#include <unordered_set>
#include <imgui.h>
#include "serialization.h"
#include "serializationHelper.h"
#include "gameObject.h"

void showComponent(Component* component, SerializationHelper& serializationHelper) {
    // ImGui::Text("New Component:"); // TODO: component name, maybe FieldDescription meta info type
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (!ImGui::TreeNode((void*)component, "%s", serializationHelper.componentTypeToString(component->getType()))) // TODO: these need to all have diff names, rn they're overlapping functionality
        return;

    for (FieldDescription& f : component->getFields())
    {
        switch (f.type)
        {
        case FieldType::INT:
            // ImGui::Text("Value: %d", *(int*)f.ptr);
            if (ImGui::DragInt(f.name, (int*)f.ptr))
                f.postUpdateFunction();
            break;
        case FieldType::FLOAT:
            // ImGui::Text("Value: %f", *(float*)f.ptr); // would need to call update matrix here too, but then this slider would need to be specific to changing transform and non generic
            // ImGui::Text("Address: %p", f.ptr);
            if (ImGui::DragFloat(f.name, (float*)f.ptr))
                f.postUpdateFunction();
            break;
        case FieldType::BOOL:
            if (ImGui::Checkbox(f.name, (bool*)f.ptr))
                f.postUpdateFunction();
            break;
        case FieldType::SUBCLASS:
            // Prob don't actually need to do this subclass thing b/c can just use treenode at top
            // ImGui::TreePush(f.name); // TODO: this will still cause probs if multiple fields w/ same name
            ImGui::BulletText("Subclass: %s", f.name);
            break;
        case FieldType::END_SUBCLASS:
            // ImGui::TreePop(); // crashing bc subclass only pushes if interacted?
            // ImGui::TreeNode("End Subclass: %s", f.name);
            break;
        case FieldType::COMPONENT_TYPE:
            // Do I still need this?
            ImGui::Text("Component: %s", f.name);
            break;
        case FieldType::VEC3:
            if (ImGui::DragFloat3(f.name, (float*)f.ptr))
                if (f.postUpdateFunction)
                    f.postUpdateFunction();
                // ((ObjectTransform*)f.objectPtr)->updateMatrix(); // why does this work without this??
        }

    }
    ImGui::TreePop();
}

// TODO: component tab should be open by default
void showGuiInspectorPanel(const std::unordered_set<GameObject*>& selectedObjects, SerializationHelper& serializationHelper)
{
    ImGui::Begin("Inspector");

    if (selectedObjects.size() == 0)
    {
        ImGui::Text("No object selected");
    }
    else if (selectedObjects.size() == 1) {
        GameObject* gameObject = *selectedObjects.begin();

        showComponent(&gameObject->transform, serializationHelper);

        for (Component *component : gameObject->getComponents())
        {
            showComponent(component, serializationHelper);
        }
        for (ComponentVisual* visualComponent : gameObject->getVisualComponents())
        {
            showComponent(visualComponent, serializationHelper);
        }

        ImGui::Button("Add Component");
        auto componentTypeNames = serializationHelper.getComponentTypeNames();
        for (auto& name : *componentTypeNames)
        {
            if (ImGui::Selectable(name))
            {
                // TODO: this should all be done inside GameObject::newComponentByType, once serializationHelper is global static
                enum ComponentType type = serializationHelper.stringToComponentType(name);
                Component* newComponent = serializationHelper.getNewComponent(type);
                if (newComponent->isVisual())
                    gameObject->addVisualComponent((ComponentVisual*)newComponent);
                else
                    gameObject->addComponent(serializationHelper.getNewComponent(type));
            }
        }
    }
    else {
        ImGui::Text("Multiple objects selected");
    }
    
    ImGui::End();
}