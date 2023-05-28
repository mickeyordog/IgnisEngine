#include <stdio.h>
#include <iostream>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
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
#include "objectTransform.h" // how to not need to do this/get around this?


// TODO: extension that lets you add includes more easily
// TODO: fix window resizing
// TODO: allow editing gameobject fields in editor
// TODO: set up camera, render scene to texture, render texture to screen
// TODO: set up scene editor
// TODO: fancier scene rendering, eg 3D stuff
// TODO: script embed/implementation
int main(int argc, char* args[])
{
	ignis_engine::Transform t;

	std::cout << "Size: " << t.getFields().size() << std::endl;
	for (const FieldDescription& f : t.getFields()) {
		std::cout << "Name: " << f.name << std::endl;
		std::cout << "Type enum: " << (int)f.type << std::endl;
		std::cout << "Value: " << *(float*)f.ptr << std::endl;
	}
	// PythonEngine pythonEngine;

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
	while(!quit)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0)
		{
			// TODO: io.WantMouseCapture etc for imgui
			ImGui_ImplSDL2_ProcessEvent(&e);
			sdlContext.handleEvents(&e);
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
		}
		InputHandler::getInstance().updateKeys();

		deltaTime = frameTimer.read();
		frameTimer.reset();
		// std::cout << 1.0f/deltaTime << " fps" << std::endl;

		dearImGuiContext.newFrame();

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
		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window); // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::Image((void *)(uintptr_t)texture.textureHandle, ImVec2(texture.width, texture.height));
			ImGui::End();
		}

		glContext.clear(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		dearImGuiContext.render();

		sdlContext.swapWindow();
	}

	return 0;
}