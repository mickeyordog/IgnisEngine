#include "rigidBodyComponent.h"
#include "gameObject.h"
#include "scene.h"

RigidBodyComponent::RigidBodyComponent(rp3d::PhysicsCommon* physicsCommon, rp3d::PhysicsWorld* world) : physicsCommon(physicsCommon), world(world) {
    fields.insert(fields.begin(), Component::getFields().begin(), Component::getFields().end());

    body = world->createRigidBody(rp3d::Transform::identity());
    body->setUserData(this);
}

RigidBodyComponent::~RigidBodyComponent() {

}

void RigidBodyComponent::start() { 

}

void RigidBodyComponent::update(float dt) { 
    rp3d::Transform transform = rp3d::Transform::interpolateTransforms(prevTransform, body->getTransform(), gameObject->scene->frameAlpha);
    rp3d::Vector3 pos = transform.getPosition();
    rp3d::Quaternion rot = transform.getOrientation();
    gameObject->transform->set({ pos.x, pos.y, pos.z }, { rot.x, rot.y, rot.z, rot.w });
    prevTransform = body->getTransform();
}
