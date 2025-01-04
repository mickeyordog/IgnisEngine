#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif

#include <SDL.h>
#include "SDLContext.h"
#include <inputHandler.h>
#include <imgui/imgui_impl_sdl2.h>

// THERE WAS MORE OS SPECIFIC STUFF IN EXAMPLE, CHECK THAT IF OS NOT WORKING
SDLContext::SDLContext(const char* name, int width, int height)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
	}
	
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    this->glsl_version = "#version 100";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
    // GL 3.2 Core + GLSL 150
    this->glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
    // GL 3.0 + GLSL 130
    this->glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

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

	this->window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE );
	if (this->window == NULL)
	{
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
	}
}

SDLContext::~SDLContext()
{
	SDL_DestroyWindow(this->window);
	SDL_Quit();
}

void SDLContext::handleEvents(bool& quit, bool& inGameView)
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
                mouseDelta = Vec2(e.motion.xrel, e.motion.yrel);
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

void SDLContext::swapWindow()
{
	SDL_GL_SwapWindow(this->window);
}

void SDLContext::captureMouse(bool capture) { 
    if (capture) {
        SDL_SetRelativeMouseMode(SDL_TRUE);
        ImGui::SetNextFrameWantCaptureMouse(false);
        ImGui::SetNextFrameWantCaptureKeyboard(false);
    } else {
        SDL_SetRelativeMouseMode(SDL_FALSE);
        // ImGui::SetWindowFocus(nullptr);
    }
}

SDL_Window *SDLContext::getWindow()
{
    return this->window;
}