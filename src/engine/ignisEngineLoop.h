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
#include "geometry.h"
// #include "pythonEngine.h"
#include "objectTransform.h"
#include "ignisEngineGui.h"
#include "scene.h"
#include "engineGuiManager.h"
#include "serialization.h"
#include "cameraComponent.h"


#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

void beginEngineMainLoop() {
    GameObject g0("g0");
    GameObject g1("g1");
    GameObject g2("g2");
    GameObject g3("g3");
    GameObject g4("g4");

    GameObject camera("Camera");
    CameraComponent cameraComponent;
    camera.addComponent(&cameraComponent);
    camera.transform.translate(Vec3{0,0,3});
    camera.transform.lookAt(Vec3{0,0,0}, Vec3{0,1,0});
    auto eeee = camera.transform.getData()[0][0];
    std::cout << eeee << std::endl;

    g0.transform.addChildTransform(g1.transform);
    g0.transform.addChildTransform(g4.transform);
    g0.transform.addChildTransform(g3.transform);

    Scene scene;
    scene.addRootGameObject(g0);
    scene.addRootGameObject(g2);
    scene.addRootGameObject(camera);


    SDLContext sdlContext("Ignis Engine", 800, 800);
    GLContext glContext(&sdlContext);
    DearImGuiContext dearImGuiContext(&sdlContext, &glContext);

    static bool show_demo_window = false;
    bool show_another_window = false;

    // glViewport(0, 0, 200, 100);
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    Texture texture("../assets/fire_penguin.png");
    Shader shader("../src/shader/vertex.vs", "../src/shader/fragment.fs");
    Geometry geometry(&texture, &shader);

    Timer frameTimer;
    float deltaTime = 0;

    Timer runtimeTimer;

    bool quit = false;
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (!quit)
#endif
    {
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

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
        // model = glm::rotate(model, glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));          
        
        const float radius = 10.0f;
        float camX = cos(runtimeTimer.read()) * radius;
        float camZ = sin(runtimeTimer.read()) * radius;
        glm::mat4 view = camera.transform.getData();
        // view = glm::lookAt(glm::vec3(camX, camZ, 3.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

        glm::mat4 projection;
        // need to adjust this to my viewport dimensions tho
        projection = glm::perspective(glm::radians(45.0f), 800.0f / 800.0f, 0.1f, 100.0f);

        shader.setUniform("model", model);
        shader.setUniform("view", view);
        shader.setUniform("projection", projection);
        geometry.render();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
        // model = glm::rotate(model, - glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));          

        shader.setUniform("model", model);
        shader.setUniform("view", view);
        shader.setUniform("projection", projection);
        geometry.render();
        
        sdlContext.swapWindow();
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif
}