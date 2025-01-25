#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif

#include <SDL.h>
#include <SDL_vulkan.h>
#include <vulkan/vulkan.hpp>
#include "windowContext.h"
#include "inputHandler.h"
#include <imgui/imgui_impl_sdl2.h>
#include <iostream>

// THERE WAS MORE OS SPECIFIC STUFF IN EXAMPLE, CHECK THAT IF OS NOT WORKING
void WindowContext::init(const char* name, int width, int height)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
        IGNIS_ERROR((std::string("SDL could not initialize! SDL Error: ") + SDL_GetError()).c_str());
	}
	
#if IGNIS_USE_VULKAN
    if (SDL_Vulkan_LoadLibrary(nullptr) < 0) {
        IGNIS_ERROR((std::string("SDL could not load Vulkan! SDL Error: ") + SDL_GetError()).c_str());
    }
    SDL_Window* window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
    if (window == nullptr)
    {
        IGNIS_ERROR((std::string("Window could not be created! SDL Error: ") + SDL_GetError()).c_str());
    }
    getInstance().window = window;
    
#else
    // GL 3.0 + GLSL 130
    getInstance().glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

#ifdef DEBUG
    int contextFlags;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_FLAGS, &contextFlags);
    contextFlags |= SDL_GL_CONTEXT_DEBUG_FLAG;
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, contextFlags);
#endif

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    getInstance().window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE );
	if (getInstance().window == NULL)
	{
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
	}
#endif
}

void WindowContext::deinit()
{
    SDL_DestroyWindow(getInstance().window);
#if IGNIS_USE_VULKAN
    SDL_Vulkan_UnloadLibrary();
#endif
    SDL_Quit();
}

void WindowContext::handleEvents(bool& quit, bool& inGameView)
{
    InputHandler::getInstance().updateState(nullptr, Vec2(0.0f));
    Vec2 mouseDelta = Vec2(0.0f);
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        ImGui_ImplSDL2_ProcessEvent(&e);
        if (e.type == SDL_QUIT) {
            quit = true;
        } else if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED) {
            int width = e.window.data1;
            int height = e.window.data2;
            glViewport(0, 0, width, height);
            // Update any other necessary components here, such as projection matrices. Will need to pass this to camera
        }
        if (!ImGui::GetIO().WantCaptureMouse && inGameView) {
            if (e.type == SDL_MOUSEMOTION && SDL_GetRelativeMouseMode() == SDL_TRUE) {
                mouseDelta = Vec2((float)e.motion.xrel, (float)e.motion.yrel);
            }
        }
        if (!ImGui::GetIO().WantCaptureKeyboard && inGameView) {
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    captureMouse(false);
                    inGameView = false;
                }
            }
        }
    }
    const Uint8* currentKeyStates = nullptr;
    if (!ImGui::GetIO().WantCaptureKeyboard) {
        currentKeyStates = SDL_GetKeyboardState(nullptr);
    }
    InputHandler::getInstance().updateState(currentKeyStates, mouseDelta);
}

void WindowContext::swapWindow()
{
	SDL_GL_SwapWindow(this->window);
}

void WindowContext::captureMouse(bool capture) { 
    if (capture) {
        SDL_SetRelativeMouseMode(SDL_TRUE);
        ImGui::SetNextFrameWantCaptureMouse(false);
        ImGui::SetNextFrameWantCaptureKeyboard(false);
    } else {
        SDL_SetRelativeMouseMode(SDL_FALSE);
        // ImGui::SetWindowFocus(nullptr);
    }
}

SDL_Window *WindowContext::getWindow()
{
    return this->window;
}

void WindowContext::showError(const char* message)
{
    std::cerr << message << std::endl;
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", message, nullptr);
    exit(EXIT_FAILURE);
}
