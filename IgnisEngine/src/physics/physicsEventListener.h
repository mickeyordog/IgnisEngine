#pragma once

#include <reactphysics3d/reactphysics3d.h>

class PhysicsEventListener : public rp3d::EventListener {
    virtual void onContact(const CollisionCallback::CallbackData& callbackData) override;
    virtual void onTrigger(const rp3d::OverlapCallback::CallbackData& callbackData) override;
};