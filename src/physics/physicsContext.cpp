#include "physicsContext.h"

PhysicsContext::PhysicsContext(bool debugEnabled) : debugEnabled(debugEnabled) {
    world = physicsCommon.createPhysicsWorld();

    // Create a rigid body in the world
    rp3d::Vector3 position(0, 5, 0);
    rp3d::Quaternion orientation = rp3d::Quaternion::identity();
    rp3d::Transform transform(position, orientation);
    body = world->createRigidBody(transform);

    floor = world->createRigidBody(rp3d::Transform({0, 0, 0}, rp3d::Quaternion::identity()));
    floor->setType(rp3d::BodyType::STATIC);
    floor->addCollider(physicsCommon.createBoxShape(rp3d::Vector3(10, 0.1, 10)), rp3d::Transform::identity());

    body->addCollider(physicsCommon.createSphereShape(1.0), rp3d::Transform::identity());
    body->getCollider(0)->getMaterial().setBounciness(1.0);
    // world->setIsGravityEnabled(false);
    if (debugEnabled) {
        floor->setIsDebugEnabled(true);
        body->setIsDebugEnabled(true);
        world->setIsDebugRenderingEnabled(true);
        debugRenderer = &world->getDebugRenderer();
        // Could allow configuring this in gui somewhere
        debugRenderer->setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::COLLIDER_AABB, true);
        debugRenderer->setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::CONTACT_POINT, true);
        debugRenderer->setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::COLLISION_SHAPE, true);
    }
}

PhysicsContext::~PhysicsContext() {
    physicsCommon.destroyPhysicsWorld(world);
}

void PhysicsContext::update(double deltaTime) { 
    const rp3d::decimal timeStep = deltaTime;

    world->update(timeStep);

    // Get the updated position of the body
    const rp3d::Transform& transform = body->getTransform();
    const rp3d::Vector3& position = transform.getPosition();

    // Display the position of the body
    // std::cout << "Body Position: (" << position.x << ", " <<
    //     position.y << ", " << position.z << ")" << std::endl;
}
