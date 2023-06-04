#pragma once

#include <stdio.h>
#include <iostream>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "SDLContext.h"
#include "GLContext.h"
#include "dearImGuiContext.h"
#include "gameObject.h"
#include "timer.h"
#include "inputHandler.h"
#include "shader.h"
#include "texture.h"
#include "spriteRenderer.h"
// #include "pythonEngine.h"
#include "objectTransform.h"
#include "ignisEngineGui.h"
#include "scene.h"
#include "engineGuiManager.h"
#include "serialization.h"
#include "serializationHelper.h"
#include "cameraComponent.h"
#include "renderTexture.h"

#ifdef __EMSCRIPTEN__
// #include "../libs/emscripten/emscripten_mainloop_stub.h" // uncomment later
#endif

void beginEngineMainLoop()
{


    // std::function<GameObject*(void)> func = []() { return new GameObject("test"); };
    // auto go0 = func();
    // auto* go1 = func();
    // go1->start();
    // return;

    SDLContext sdlContext("Ignis Engine", 800, 800);
    GLContext glContext(&sdlContext);
    DearImGuiContext dearImGuiContext(&sdlContext, &glContext);

    Texture texture("../assets/fire_penguin.png");
    Shader shader("../src/shader/vertex.vs", "../src/shader/fragment.fs");


    // this can probably just be global static, or doesn't even need to be a class
    SerializationHelper serializationHelper; // TODO: replace this with registering ComponentClassInfo for each class, then don't have to do that weird mapping to string thing
    serializationHelper.registerComponentClass(ComponentType::CAMERA, []() { return new CameraComponent(800, 800); });
    serializationHelper.registerComponentClass(ComponentType::TRANSFORM, []() { return new ObjectTransform(); });
    serializationHelper.registerComponentClass(ComponentType::SPRITE_RENDERER, [&]() { return new SpriteRenderer(&texture, &shader); });

    SerializationHelper::registerComponentClass({ ComponentType::CAMERA, "Camera", []() { return new CameraComponent(800, 800); } });
    SerializationHelper::registerComponentClass({ ComponentType::TRANSFORM, "Transform", []() { return new ObjectTransform(); } });
    SerializationHelper::registerComponentClass({ ComponentType::SPRITE_RENDERER, "Sprite Renderer", [&]() { return new SpriteRenderer(&texture, &shader); } });

    // TODO: should prob just make serializationHelper some kind of global so don't need to pass it literally everywhere, and then gameobject can just have a add component of type method
    GameObject g0("g0");
    SpriteRenderer* sr0 = (SpriteRenderer*)serializationHelper.getNewComponent(ComponentType::SPRITE_RENDERER); // NEED TO FREE THESE TOO!
    g0.addVisualComponent(sr0);
    GameObject g1("g1");
    SpriteRenderer* sr1 = (SpriteRenderer*)serializationHelper.getNewComponent(ComponentType::SPRITE_RENDERER);
    g1.addVisualComponent(sr1);
    g1.transform.translate(Vec3 { -1,0,0 });
    GameObject g2("g2");
    GameObject g3("g3");
    GameObject g4("g4");

    GameObject camera("Camera");
    CameraComponent* cameraComponent = (CameraComponent*)serializationHelper.getNewComponent(ComponentType::CAMERA);
    camera.addComponent(cameraComponent);
    camera.transform.translate(Vec3 { 0,0,5 });
    camera.transform.lookAt(Vec3 { 0,0,0 }, Vec3 { 0,1,0 });
    // camera.transform.lookAt(Vec3 { 0,0,0 }, Vec3 { 0,1,0 });
    // camera.transform.translate(Vec3 { 0,-1,0 });

    g0.transform.addChildTransform(g1.transform);
    g0.transform.addChildTransform(g2.transform);
    g0.transform.addChildTransform(g3.transform);
    g3.transform.addChildTransform(g4.transform); // Opening this causes error for some reason

    Scene scene;
    scene.addRootGameObject(&camera); // TODO: replace this with method that constructs and creates gameobject pointer itself
    scene.addRootGameObject(&g0);

    static bool show_demo_window = false;
    bool show_another_window = false;

    // glViewport(0, 0, 200, 100);
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


    Timer frameTimer;
    float deltaTime = 0;

    Timer runtimeTimer;
    scene.startGameObjects();

    bool quit = false;
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (!quit)
#endif
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

        // camera.transform.lookAt(Vec3 { 0,0,0 }, Vec3 { 0,1,0 });
        // std::cout << "g0 position: " << g0.transform.getPosition().getData().x << ", " << g0.transform.getPosition().getData().y << ", " << g0.transform.getPosition().getData().z << std::endl;

        scene.updateGameObjects(deltaTime);
        cameraComponent->renderScene(scene);
        showIgnisEngineGui(scene, cameraComponent->getOutputTexture(), serializationHelper);

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
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window); // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            // ImGui::Image((void*)(uintptr_t)renderTexture.texture, ImVec2(renderTexture.width, renderTexture.height));
            ImGui::End();
        }
#pragma endregion

        glContext.clear(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        dearImGuiContext.render();

        sdlContext.swapWindow();
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif
}