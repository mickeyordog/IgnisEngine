#pragma once

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
#include "spriteRenderer.h"
#include "meshRenderer.h"
#include "firstPersonController.h"
#include "transformComponent.h"
#include "ignisEngineGui.h"
#include "scene.h"
#include "engineGuiManager.h"
#include "serialization.h"
#include "serializationHelper.h"
#include "animatorComponent.h"
#include "renderTexture.h"
#include "assetManager.h"
#include "lightComponent.h"

#include <imfilebrowser.h>

#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h" // uncomment later
#endif

void processInput(SDLContext& sdlContext, bool& quit) {
    // TODO: move to sdlContext
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {
        // TODO: io.WantCaptureMouse wantCaptureKeyboard etc for imgui
        ImGui_ImplSDL2_ProcessEvent(&e);
        sdlContext.handleEvents(&e);
        if (e.type == SDL_QUIT)
        {
            quit = true;
        } else if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED)
        {
            int width = e.window.data1;
            int height = e.window.data2;
            glViewport(0, 0, width, height);
            // Update any other necessary components here, such as projection matrices. Will need to pass this to camera
        }
    }
    int mouseX, mouseY;
    // This isn't currently working, thinks imgui using mouse when I'm over scene render texture
    // if (ImGui::GetIO().WantCaptureMouse) {
    //     mouseX = (int)InputHandler::getInstance().getMousePos().x();
    //     mouseY = (int)InputHandler::getInstance().getMousePos().y();
    // } else {
        SDL_GetMouseState(&mouseX, &mouseY);
    // }
    InputHandler::getInstance().updateState(SDL_GetKeyboardState(nullptr), Vec2(mouseX, mouseY));
}

#include <thread>
#include <chrono>
#include <future>
#include <mutex>
void beginEngineMainLoop()
{
    SDLContext sdlContext("Ignis Engine", 800, 800);
    GLContext glContext(&sdlContext);
    DearImGuiContext dearImGuiContext(&sdlContext, &glContext);

    AssetManager::recursivelyRegisterAllAssetsInDirectory("../assets");

// TODO: try writing and loading a couple big files async vs not and see if time difference
/*
#pragma region TEST
    Timer testTimer;
    std::chrono::microseconds duration(1);
    int size = 100;
    auto printArray = [&](int array[]){ for (int i = 0; i < size; i++) { printf("%d,", array[i]); }; printf("\n"); };
    int array[size];
    for (int i = 0; i < size; i++) { array[i] = i; }
    // printArray(array);

    auto incArray = [&](int* array, int start, int end) { std::this_thread::sleep_for(duration); for (int i = start; i < end; i++) { array[i] += 100; };};
    int num = 0;
    int numTimes = 10;
    std::mutex m;
    auto incNum = [&](int numTimes) {
        for (int i = 0; i < numTimes; i++) {
            {
                std::lock_guard<std::mutex> lk(m);
                num++; 
            }
            std::this_thread::sleep_for(duration); 
        } 
    };
    std::unordered_map<int, int> map;
    for (int i = 0; i < numTimes; i++) { map[i] = i; }
    auto findAndInc = [&]{
        for (int i = 0; i < numTimes; i++) {
            if (map[i] < i * 100) {
                std::lock_guard<std::mutex> lk(m);
                int n = map[i];
                std::this_thread::sleep_for(duration);
                map[i] *= 10;
                std::cout << "Thread " << std::this_thread::get_id() << " changed map[" << i << "]" << std::endl;
            }
        }
    };
    int numThreads = 10;
    std::future<void> futureArray[numThreads];
    for (int i = 0; i < numThreads; i++)
    {
        // futureArray[i] = std::async(std::launch::async, incArray, (int*)array, 0, 100);
        futureArray[i] = std::async(std::launch::async, findAndInc);
    }
    for (int i = 0; i < numThreads; i++)
    {
        futureArray[i].wait();
        printf("%d Done at %f\n", i, testTimer.read());
    }
    printf("Done all %f\n", testTimer.read());
    for (int i = 0; i < numTimes; i++) {
        printf("Map[%d] = %d, ", i, map[i]);
    }
    // printArray(array);
    // printf("Expected Num: %d, Actual Num: %d\n", numThreads * numTimes, num);
    // Timer testTimer;
    // std::chrono::seconds duration(1);
    // auto func = [](){};
    // auto f1 = std::async(std::launch::async, [&] { std::this_thread::sleep_for(duration); printf("1 Done! Time: %f\n", testTimer.read()); });
    // auto f2 = std::async(std::launch::async, [&] { std::this_thread::sleep_for(duration); printf("2 Done! Time: %f\n", testTimer.read()); });
    // f1.wait();
    // f2.wait();
    // printf("Total Time: %f\n", testTimer.read());


#pragma endregion
*/

    SerializationHelper::registerComponentClass({ ComponentType::CAMERA, "Camera", []() { return new CameraComponent(800, 800); } });
    SerializationHelper::registerComponentClass({ ComponentType::TRANSFORM, "Transform", []() { return new TransformComponent(); } });
    SerializationHelper::registerComponentClass({ ComponentType::SPRITE_RENDERER, "Sprite Renderer", []() { return new SpriteRenderer((Texture*)AssetManager::loadOrGetAsset(1), (Shader*)AssetManager::loadOrGetAsset(523457802578)); } });
    SerializationHelper::registerComponentClass({ ComponentType::ANIMATOR, "Animator", []() { return new AnimatorComponent(); } });
    SerializationHelper::registerComponentClass({ ComponentType::MESH_RENDERER, "Mesh Renderer", []() { return new MeshRenderer((Model*)AssetManager::loadOrGetAsset(11645431234), (Shader*)AssetManager::loadOrGetAsset(6016548490632296826)); } });
    SerializationHelper::registerComponentClass({ ComponentType::FIRST_PERSON_CONTROLLER, "First Person Controller", []() { return new FirstPersonController(5.0, 90.0); } });
    SerializationHelper::registerComponentClass({ ComponentType::LIGHT, "Light", []() { return new LightComponent(LightType::SPOTLIGHT); } });

    /*
    GameObject g0("g0");
    g0.addComponentOfType(ComponentType::SPRITE_RENDERER);
    GameObject g1("g1");
    g1.addComponentOfType(ComponentType::SPRITE_RENDERER);
    g1.transform->translate(Vec3 { -1,0,0 });
    GameObject g2("g2");
    g2.addComponentOfType(ComponentType::SPRITE_RENDERER);
    g2.transform->translate(Vec3 { 1,0,0 });
    GameObject g3("g3");
    GameObject g4("g4");

    GameObject camera("Camera");
    camera.addComponentOfType(ComponentType::CAMERA);
    CameraComponent* cameraComponent = (CameraComponent*)camera.getComponentOfType(ComponentType::CAMERA);
    camera.transform->translate(Vec3 { 0,0,5 });
    camera.transform->lookAt(Vec3 { 0,0,0 }, Vec3 { 0,1,0 });

    g0.transform->addChildTransform(g1.transform);
    g0.transform->addChildTransform(g2.transform);
    g0.transform->addChildTransform(g3.transform);
    g3.transform->addChildTransform(g4.transform); // Opening this causes error for some reason

    Scene scene;
    scene.addRootGameObject(&camera); // TODO: replace this with method that constructs and creates gameobject pointer itself
    scene.addRootGameObject(&g0);

    SerializationHelper::serializeScene(scene);
    */
    
    // Scene currently has two first person controllers on camera, how did that happen when I loaded scene?
    Scene scene = *(Scene*)AssetManager::loadOrGetAsset(43540);
    // scene.mainCamera->gameObject->addComponentOfType(ComponentType::FIRST_PERSON_CONTROLLER);
    // GameObject* cube = new GameObject("Cube");
    // cube->addComponentOfType(ComponentType::MESH_RENDERER);
    // scene.addRootGameObject(cube);
    GameObject* dirLight = new GameObject("DirLight");
    LightComponent* lightComponent = (LightComponent*)dirLight->addComponentOfType(ComponentType::LIGHT);
    lightComponent->lightType = LightType::SPOTLIGHT;
    scene.addRootGameObject(dirLight);
    scene.lights.push_back(lightComponent);

    // scene runtime file should hold onto the lights being used (rn will just be all of them),
    // then ig could pass them to the camera for rendering. But render loop should also be taken out of camera

#pragma region lightmapper
//     lm_context* ctx = lmCreate(
//         64,               // hemicube rendering resolution/quality
//         0.001f, 100.0f,   // zNear, zFar
//         1.0f, 1.0f, 1.0f, // sky/clear color
//         2, 0.01f,         // hierarchical selective interpolation for speedup (passes, threshold)
//         0.0f);            // modifier for camera-to-surface distance for hemisphere rendering.
//     // tweak this to trade-off between interpolated vertex normal quality and other artifacts (see declaration).
//     if (!ctx)
//     {
//         printf("Could not initialize lightmapper.\n");
//         exit(-1);
//     }

//     for (int b = 0; b < bounces; b++)
//     {
//         // render all geometry to their lightmaps
//         for (int i = 0; i < meshes; i++)
//         {
//             memset(mesh[i].lightmap, 0, sizeof(float) * mesh[i].lightmapWidth * mesh[i].lightmapHeight * 3); // clear lightmap to black
//             lmSetTargetLightmap(ctx, mesh[i].lightmap, mesh[i].lightmapWidth, mesh[i].lightmapHeight, 3);

//             lmSetGeometry(ctx, mesh[i].modelMatrix,
//                           LM_FLOAT, (uint8_t*)mesh[i].vertices + positionOffset, vertexSize,
//                           LM_NONE, NULL, 0, // optional vertex normals for smooth surfaces
//                           LM_FLOAT, (uint8_t*)mesh[i].vertices + lightmapUVOffset, vertexSize,
//                           mesh[i].indexCount, LM_UNSIGNED_SHORT, mesh[i].indices);

//             int vp[4];
//             mat4 view, proj;
//             while (lmBegin(ctx, vp, &view[0][0], &proj[0][0]))
//             {
//                 // don't glClear on your own here!
//                 glViewport(vp[0], vp[1], vp[2], vp[3]);
//                 drawScene(view, proj);
//                 printf("\r%6.2f%%", lmProgress(ctx) * 100.0f); // don't actually call printf that often ;) it's slow.
//                 lmEnd(ctx);
//             }
//     }

//         // postprocess and upload all lightmaps to the GPU now to use them for indirect lighting during the next bounce.
//         for (int i = 0; i < meshes; i++)
//         {
//             // you can also call other lmImage* here to postprocess the lightmap.
//             lmImageDilate(mesh[i].lightmap, temp, mesh[i].lightmapWidth, mesh[i].lightmapHeight, 3);
//             lmImageDilate(temp, mesh[i].lightmap, mesh[i].lightmapWidth, mesh[i].lightmapHeight, 3);

//             glBindTexture(GL_TEXTURE_2D, mesh[i].lightmapHandle);
//             glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mesh[i].lightmapWidth, mesh[i].lightmapHeight, 0, GL_RGB, GL_FLOAT, data);
//         }
// }

//     lmDestroy(ctx);

//     // gamma correct and save lightmaps to disk
//     for (int i = 0; i < meshes; i++)
//     {
//         lmImagePower(mesh[i].lightmap, mesh[i].lightmapWidth, mesh[i].lightmapHeight, 3, 1.0f / 2.2f);
//         lmImageSaveTGAf(mesh[i].lightmapFilename, mesh[i].lightmap, mesh[i].lightmapWidth, mesh[i].lightmapHeight, 3);
//     }
    #pragma endregion

    // glViewport(0, 0, 200, 100);
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    Timer frameTimer;
    float deltaTime = 0;

    scene.startGameObjects();

#pragma region imfilebrowser
    ImGui::FileBrowser fileDialog(ImGuiFileBrowserFlags_NoModal);
    // (optional) set browser properties
    fileDialog.SetTitle("title");
    fileDialog.SetTypeFilters({ ".h", ".cpp" });
#pragma endregion

    bool quit = false;
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (!quit)
#endif
    {
        processInput(sdlContext, quit);

        deltaTime = frameTimer.readAndReset();
        // std::cout << 1.0f/deltaTime << " fps" << std::endl;
        dearImGuiContext.newFrame();

#pragma region imfilebrowser
        if (ImGui::Begin("dummy window"))
        {
            // open file dialog when user clicks this button
            if (ImGui::Button("open file dialog"))
                fileDialog.Open();
        }
        ImGui::End();
        fileDialog.Display();
        if (fileDialog.HasSelected())
        {
            std::cout << "Selected filename" << fileDialog.GetSelected().string() << std::endl;
            fileDialog.ClearSelected();
        }
#pragma endregion
#pragma region Dear Imgui Remove This
        static bool show_demo_window = false;
        static bool show_another_window = false;
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

        runIgnisEngineGui(scene);

        // TODO: also need fixed timestep
        scene.updateGameObjects(deltaTime); // this should actually only happen in game build or during play mode
        glContext.clear(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        scene.render();
        dearImGuiContext.render();

        sdlContext.swapWindow();
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif
}