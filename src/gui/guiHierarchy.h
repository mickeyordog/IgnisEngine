#pragma once

#include <stack>
#include <unordered_set>
#include <imgui.h>
#include "scene.h"

void showGuiHierarchyPanel(Scene& scene) {
    ImGui::Begin("Hierarchy");
    if (ImGui::TreeNode("Scene"))
    {
        static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
        static bool test_drag_and_drop = true;

        // 'selection_mask' is dumb representation of what may be user-side selection state.
        //  You may retain selection state inside or outside your objects in whatever format you see fit.
        // 'node_clicked' is temporary storage of what node we have clicked to process selection at the end
        /// of the loop. May be a pointer to your own node type, etc.
        static int selection_mask = (1 << 2);
        GameObject* node_clicked = nullptr;
        static std::unordered_set<GameObject*> selectedObjects;
        std::stack<GameObject*> objectStack;
        std::stack<GameObject*> treePopStack;
        for (auto gameObject = scene.getRootGameObjects().rbegin(); gameObject != scene.getRootGameObjects().rend(); ++gameObject)
        {
            objectStack.push(*gameObject); // TODO: does this copy them? Should use ptrs instead?
        }
        while (objectStack.size() > 0) {
            GameObject* currentObject = objectStack.top();
            objectStack.pop();
            
            // Disable the default "open on single-click behavior" + set Selected flag according to our selection.
            // To alter selection we use IsItemClicked() && !IsItemToggledOpen(), so clicking on an arrow doesn't alter selection.
            ImGuiTreeNodeFlags node_flags = base_flags;
            // const bool is_selected = (selection_mask & (1 << i)) != 0;
            const bool is_selected = selectedObjects.find(currentObject) != selectedObjects.end();
            if (is_selected)
                node_flags |= ImGuiTreeNodeFlags_Selected;
            if (currentObject->getChildObjects().size() > 0)
            {
                bool node_open = ImGui::TreeNodeEx(currentObject->name, node_flags); // TODO: Do I need to pass a ptr_id here? What does that do?
                if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
                    node_clicked = currentObject;
                if (test_drag_and_drop && ImGui::BeginDragDropSource())
                {
                    ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
                    ImGui::Text("This is a drag and drop source");
                    ImGui::EndDragDropSource();
                }
                if (node_open)
                {
                    for (auto gameObject = currentObject->getChildObjects().rbegin(); gameObject != currentObject->getChildObjects().rend(); ++gameObject)
                    {
                        objectStack.push(*gameObject);
                    }
                    treePopStack.push(currentObject->getChildObjects().back());
                    
                    // ImGui::BulletText("Blah blah\nBlah Blah");
                    // ImGui::TreePop();
                }
            }
            else
            {
                // Items 3..5 are Tree Leaves
                // The only reason we use TreeNode at all is to allow selection of the leaf. Otherwise we can
                // use BulletText() or advance the cursor by GetTreeNodeToLabelSpacing() and call Text().
                node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
                ImGui::TreeNodeEx(currentObject->name, node_flags);
                if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
                    node_clicked = currentObject;
                if (test_drag_and_drop && ImGui::BeginDragDropSource())
                {
                    ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
                    ImGui::Text("This is a drag and drop source");
                    ImGui::EndDragDropSource();
                }
            }
            if (treePopStack.size() > 0 && currentObject == treePopStack.top())
            {
                ImGui::TreePop();
                treePopStack.pop();
            }
        }
        if (node_clicked != nullptr)
        {
            // Update selection state
            // (process outside of tree loop to avoid visual inconsistencies during the clicking frame)
            if (ImGui::GetIO().KeyCtrl) {
                // selection_mask ^= (1 << node_clicked);          // CTRL+click to toggle
                if (selectedObjects.find(node_clicked) != selectedObjects.end()) {
                    selectedObjects.erase(node_clicked);
                }
                else {
                    selectedObjects.insert(node_clicked);
                }
            }
            else {//if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, may want to preserve selection when clicking on item that is part of the selection
                // selection_mask = (1 << node_clicked);           // Click to single-select
                selectedObjects.clear();
                selectedObjects.insert(node_clicked);
            }
        }
        ImGui::TreePop();
    }
    ImGui::End();
}