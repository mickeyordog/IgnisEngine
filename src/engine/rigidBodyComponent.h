#pragma once

#include <reactphysics3d/reactphysics3d.h>
#include "component.h"

// Will need to get a callback from the physics system to update the transform of the game object
class RigidBodyComponent : public Component {
public:
    RigidBodyComponent();
    ~RigidBodyComponent();
private:
    rp3d::RigidBody* body;
};