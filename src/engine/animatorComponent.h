#pragma once

#include "component.h"
#include "serialization.h"
#include "animationController.h"

// REQUIREMENTS:
//  contains an AnimationController that holds all the AnimationClips and updates everything
//
class AnimatorComponent : public Component {
public:
    AnimatorComponent();
    ~AnimatorComponent();

    AnimationController* controller;

    virtual void start() override;
    virtual void update(float dt) override;
    virtual enum ComponentType getType() override { return ComponentType::ANIMATOR; };

    virtual std::vector<FieldDescription>& getFields() override { return fields; };

private:
    std::vector<FieldDescription> fields = {
        { GET_NAME(controller), FieldType::ASSET_POINTER_FIELD, &controller}
     };
};