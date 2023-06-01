#include <stdio.h>
#include <iostream>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include <vector>
#include <string>
#include "SDLContext.h"
#include "GLContext.h"
#include "dearImGuiContext.h"
#include "gameObject.h"
#include "timer.h"
#include "inputHandler.h"
#include "shader.h"
#include "texture.h"
#include "geometry.h"
#include "pythonEngine.h"
#include "objectTransform.h"
#include "ignisEngineGui.h"
#include "scene.h"
#include "engineGuiManager.h"
#include "serialization.h"


// TODO: extension that lets you add includes more easily
// TODO: fix window resizing
// TODO: allow editing gameobject fields in editor
// TODO: set up camera, render scene to texture, render texture to screen
// TODO: set up scene editor
// TODO: fancier scene rendering, eg 3D stuff
// TODO: script embed/implementation
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

	GameObject g0("g0");
	GameObject g1("g1");
	GameObject g2("g2");
	GameObject g3("g3");
	GameObject g4("g4");
	ignis_engine::Transform t0(0, 0, 0);
	ignis_engine::Transform t1(0, 0, 0);
	ignis_engine::Transform t2(0, 0, 0);
	g0.addComponent(t0);
	g1.addComponent(t1);
	g2.addComponent(t2);
	g0.addChildObject(g1);
	g0.addChildObject(g4);
	g1.addChildObject(g3);

	// Print out the locations of each of the three Vec3's in g0
	// printf("%p\n%p\n%p\n", &t0.position.x, &t0.position.y, &t0.position.z);

	Scene scene;
	scene.addRootGameObject(g0);
	scene.addRootGameObject(g2);

	// for (Component* component : g0.getComponents()) {
	// 	std::cout << "New Component:" << std::endl; // TODO: component name, maybe FieldDescription meta info type
	// 	for (const FieldDescription& f : component->getFields())
	// 	{
	// 		std::cout << "Field name: " << f.name << std::endl;
	// 		std::cout << "Type enum: " << (int)f.type << std::endl;
	// 		std::cout << "Value: " << *(float*)f.ptr << std::endl << std::endl;
	// 	}
	// }

	SDLContext sdlContext("Ignis Engine", 1280, 720);
	GLContext glContext(&sdlContext);
	DearImGuiContext dearImGuiContext(&sdlContext, &glContext);

	bool show_demo_window = true;
	bool show_another_window = false;

	// glViewport(0, 0, 200, 100);
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	Texture texture("../assets/texture.png");
	Shader shader("../src/shader/vertex.vs", "../src/shader/fragment.fs");
	Geometry geometry(&texture, &shader);

	Timer frameTimer;
	float deltaTime = 0;

	bool quit = false;
	while (!quit) {
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0) {
			// TODO: io.WantMouseCapture etc for imgui
			ImGui_ImplSDL2_ProcessEvent(&e);
			sdlContext.handleEvents(&e);
			if (e.type == SDL_QUIT) {
				quit = true;
			}
		}
		InputHandler::getInstance().updateKeys();

		deltaTime = frameTimer.read();
		frameTimer.reset();
		// std::cout << 1.0f/deltaTime << " fps" << std::endl;

		dearImGuiContext.newFrame();

		showIgnisEngineGui(scene);

#pragma region Dear Imgui Remove This
		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");		   // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);			 // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			float framerate = dearImGuiContext.getIO().Framerate;
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / framerate, framerate);
			ImGui::End();
		}

		// 3. Show another simple window.
		if (show_another_window) {
			ImGui::Begin("Another Window", &show_another_window); // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::Image((void*)(uintptr_t)texture.textureHandle, ImVec2(texture.width, texture.height));
			ImGui::End();
		}
#pragma endregion

		glContext.clear(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		dearImGuiContext.render();

		sdlContext.swapWindow();
	}

	return 0;
}