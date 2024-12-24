#pragma once

#include "reactphysics3d/reactphysics3d.h"
#include "renderTexture.h"
#include "shader.h"
#include "cameraComponent.h"

class PhysicsDebugRenderer {
public:
    PhysicsDebugRenderer(RenderTexture* renderTexture, Shader* lineShader, Shader* triangleShader, CameraComponent* camera, rp3d::DebugRenderer* debugRenderer);
    ~PhysicsDebugRenderer() = default;

    void draw();

private:
    void drawLines();
    void drawTriangles();
    
    RenderTexture* renderTexture;
    Shader* lineShader;
    Shader* triangleShader;
    CameraComponent* camera;
    rp3d::DebugRenderer* debugRenderer;
};