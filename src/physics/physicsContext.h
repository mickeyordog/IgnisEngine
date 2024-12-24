#pragma once

#include <reactphysics3d/reactphysics3d.h>

class PhysicsContext {
public:
    PhysicsContext(bool debugEnabled);
    ~PhysicsContext();

    void update(double deltaTime);

    rp3d::DebugRenderer* debugRenderer;

private:
    rp3d::PhysicsCommon physicsCommon;
    rp3d::PhysicsWorld* world;
    rp3d::RigidBody* body;
    bool debugEnabled;
};