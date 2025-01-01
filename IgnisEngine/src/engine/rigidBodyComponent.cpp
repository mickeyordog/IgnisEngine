#include "rigidBodyComponent.h"
#include "gameObject.h"
#include "scene.h"

RigidBodyComponent::RigidBodyComponent(PhysicsContext* physicsContext) : ctx(physicsContext) {
    fields.insert(fields.begin(), Component::getFields().begin(), Component::getFields().end());

    body = ctx->world->createRigidBody(rp3d::Transform::identity());
    body->setUserData(this);
    if (ctx->debugEnabled) {
        body->setIsDebugEnabled(true);
    }
    // would like to set transform equal to gameObject transform, but gameObject is not set until after ctor
    // could set it in start but that could override a transform set in the scene file or code
    // but that def needs to happen early at some point so positions can even be saved
    // could have GameObject* as an arg to all component creation registration fns

    body->addCollider(ctx->physicsCommon.createSphereShape(1.0), rp3d::Transform::identity());
    body->getCollider(0)->getMaterial().setBounciness(1.0);
}

RigidBodyComponent::~RigidBodyComponent() {

}

void RigidBodyComponent::start() { 

}

void RigidBodyComponent::update(float dt) { 
    rp3d::Transform transform = rp3d::Transform::interpolateTransforms(prevTransform, curTransform, gameObject->scene->frameAlpha);
    rp3d::Vector3 pos = transform.getPosition();
    rp3d::Quaternion rot = transform.getOrientation();
    gameObject->transform->set({ pos.x, pos.y, pos.z }, { rot.x, rot.y, rot.z, rot.w });
}

void RigidBodyComponent::fixedUpdate(float dt) { 
    prevTransform = curTransform;
    curTransform = body->getTransform();
}
