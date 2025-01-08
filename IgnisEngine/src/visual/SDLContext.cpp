#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif

#include <SDL.h>
#include <SDL_vulkan.h>
#include <vulkan/vulkan.hpp>
#include "SDLContext.h"
#include <inputHandler.h>
#include <imgui/imgui_impl_sdl2.h>
#include <iostream>

#define IGNIS_USE_VULKAN TRUE

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

// THERE WAS MORE OS SPECIFIC STUFF IN EXAMPLE, CHECK THAT IF OS NOT WORKING
SDLContext::SDLContext(const char* name, int width, int height)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
	}
	
#if IGNIS_USE_VULKAN
    if (SDL_Vulkan_LoadLibrary(nullptr) < 0) {
        printf("SDL could not load Vulkan! SDL Error: %s\n", SDL_GetError());
    }
    window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
    if (window == nullptr)
    {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
    }
    // From here on would prob be in vulkan context
    VkInstance instance; // Should be field

    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "My Game";
    appInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
    appInfo.pEngineName = "Ignis Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    uint32_t extensionCount;
	if (SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, nullptr) < 0) {
		throw std::runtime_error("failed to get required Vulkan extensions from SDL!");
	}
    //std::vector<const char*> extensionNames(extensionCount);
    const char** extensionNames = nullptr;
    extensionNames = new const char* [extensionCount];
	if (SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, extensionNames/*.data()*/) < 0) {
		throw std::runtime_error("failed to get required Vulkan extensions from SDL!");
	}
	//for (const char* extensionName : extensionNames) {
	//	printf("Extension: %s\n", extensionName);
	//}

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = extensionCount;
    createInfo.ppEnabledExtensionNames = extensionNames/*.data()*/;
    createInfo.enabledLayerCount = 0;
	createInfo.ppEnabledLayerNames = nullptr;
    createInfo.pNext = nullptr;
    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }

    uint32_t extensionPropCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionPropCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionPropCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionPropCount, extensions.data());
    std::cout << "available extensions:\n";
    for (const auto& extension : extensions) {
        std::cout << '\t' << extension.extensionName << '\n';
    }
    // If want to, can check that all required extensions are in this list
    



    
    // cleanup
    vkDestroyInstance(instance, nullptr);
#else
    // GL 3.0 + GLSL 130
    this->glsl_version = "#version 130";
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

	this->window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE );
	if (this->window == NULL)
	{
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
	}
#endif
}

SDLContext::~SDLContext()
{
	SDL_DestroyWindow(this->window);
#if IGNIS_USE_VULKAN
	SDL_Vulkan_UnloadLibrary();
#endif
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