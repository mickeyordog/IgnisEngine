#include "ignisEngineLoop.h"

// TODO: extension that lets you add includes more easily
// TODO: fix window resizing
// TODO: set up camera, render scene to texture, render texture to screen
// TODO: set up scene editor
// TODO: fancier scene rendering, eg 3D stuff
// TODO: could make vscode plugin that helps you add includes easily (give you window to top of class)
// TODO: and one for editing header and cpp declaration at same time (could use regex to find, then just put cursor in both places and can edit both at once)
// TODO: does cpp optimize not making a copy if you don't change value when assigning local and just use it to get some vals out?
// TODO: could it optimize if you have (x * y) * z where multiplying by x is an expensive operation but by y and z isn't?
int main(int argc, char* args[]) {
	// HIERARCHY REQUIREMENTS
	// 1. See all gameobjects in scene, nested under their respective parents
	//	- By default, only root child objects will show. Press arrow to show that object's child(ren)
	// 2. Upon selecting a gameobject, its information/fields will show up in the inspector
	//	- An arbitrary number of gameobjects can be selected at once, and their common fields will be editable
	//	- Selected gameobjects will be stored in static vector of imgui function for hierarchy
	// 3. These fields will be modifiable in real time
	// 4. Gameobjects can be rearranged in scene by dragging

	// TODO: where should I register these? How can I arrange for them to be registered outside engine for game specific components too?
	// EngineGuiManager::getInstance().registerComponentClass({ GET_NAME(ignis_engine::Transform), nullptr });

	beginEngineMainLoop();

	return 0;
}