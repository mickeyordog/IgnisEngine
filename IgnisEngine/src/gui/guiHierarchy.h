#pragma once

#include <stack>
#include <unordered_set>
#include <imgui/imgui.h>
#include "scene.h"

void showGuiHierarchyPanel(Scene& scene, std::unordered_set<GameObject*>& selectedObjects)
{
    ImGui::Begin("Hierarchy");

    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::TreeNode("Scene"))
    {
        static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        // base_flags |= ImGuiTreeNodeFlags_DefaultOpen; 
        static bool test_drag_and_drop = true;

        GameObject* node_clicked = nullptr;

        SceneIterator it = scene.getIterator();
        bool skipChildren = false;
        for (SceneIteratorInfo info = it.getNextInfo(skipChildren); info.gameObject != nullptr; info = it.getNextInfo(skipChildren))
        {
            skipChildren = false;
            GameObject* currentObject = info.gameObject;
            int depth = info.depth;
            for (int i = 0; i < depth; i++) ImGui::Indent();
            // Disable the default "open on single-click behavior" + set Selected flag according to our selection.
            // To alter selection we use IsItemClicked() && !IsItemToggledOpen(), so clicking on an arrow doesn't alter selection.
            ImGuiTreeNodeFlags node_flags = base_flags;
            const bool is_selected = selectedObjects.find(currentObject) != selectedObjects.end();
            if (is_selected)
                node_flags |= ImGuiTreeNodeFlags_Selected;
            if (currentObject->transform->getChildTransforms().size() > 0)
            {
                bool node_open = ImGui::TreeNodeEx(currentObject->name.c_str(), node_flags);
                if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
                    node_clicked = currentObject;
                if (test_drag_and_drop && ImGui::BeginDragDropSource())
                {
                    ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
                    ImGui::Text("This is a drag and drop source");
                    ImGui::EndDragDropSource();
                }
                if (!node_open) {
                    skipChildren = true;
                }
            }
            else
            {
                // Items 3..5 are Tree Leaves
                // The only reason we use TreeNode at all is to allow selection of the leaf. Otherwise we can
                // use BulletText() or advance the cursor by GetTreeNodeToLabelSpacing() and call Text().
                node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
                ImGui::TreeNodeEx(currentObject->name.c_str(), node_flags);
                if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
                    node_clicked = currentObject;
                if (test_drag_and_drop && ImGui::BeginDragDropSource())
                {
                    ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
                    ImGui::Text("This is a drag and drop source");
                    ImGui::EndDragDropSource();
                }
            }
            for (int i = 0; i < depth; i++) ImGui::Unindent();
        }
        if (node_clicked != nullptr)
        {
            if (ImGui::GetIO().KeyCtrl)
            {
                if (selectedObjects.find(node_clicked) != selectedObjects.end())
                {
                    selectedObjects.erase(node_clicked);
                }
                else
                {
                    selectedObjects.insert(node_clicked);
                }
            }
            else
            {
                selectedObjects.clear();
                selectedObjects.insert(node_clicked);
            }
        }
        ImGui::TreePop();
    }

    if (ImGui::Button("Add GameObject"))
    {
        auto* gameObject = new GameObject("GameObject");
        scene.addRootGameObject(gameObject);
    }
    ImGui::End();
}