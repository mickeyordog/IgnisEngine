#pragma once

#include <reactphysics3d/reactphysics3d.h>
#include "physicsContext.h"
#include "component.h"

class RigidBodyComponent : public Component {
public:
    RigidBodyComponent(PhysicsContext* physicsContext);
    ~RigidBodyComponent();

    virtual void start() override;
    virtual void update(float dt) override;
    virtual void fixedUpdate(float dt) override;
    virtual enum ComponentType getType() override { return ComponentType::RIGID_BODY; };
    virtual const std::vector<FieldDescription>& getFields() override { return fields; };
    // TODO: this class will have a set of vectors of std::function that you can subscribe to by adding a function
    // This class will have a method that gets called by eventlistener that will call all the functions in the vectors

private:
    rp3d::RigidBody* body;
    rp3d::Transform prevTransform;
    rp3d::Transform curTransform;

    PhysicsContext* ctx;

    std::vector<FieldDescription> fields = { };
};