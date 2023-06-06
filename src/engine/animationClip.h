#pragma once

#include "component.h"
#include "serialization.h"

enum class AnimationClipVariableType {
    FLOAT,
    INT,
    BOOL,
    TRIGGER
};

struct AnimationClipVariable {
    AnimationClipVariableType type;
    char* name;
    union {
        float floatValue;
        int intValue;
        bool boolValue;
    };
};

enum class AnimationClipConditionType {
    EQUAL,
    NOT_EQUAL,
    LESS_THAN,
    GREATER_THAN,
    LESS_THAN_OR_EQUAL,
    GREATER_THAN_OR_EQUAL,
};

struct AnimationClipCondition {
    AnimationClipVariable variable;
    AnimationClipConditionType type;
};

class AnimationClip;
struct AnimationClipTransition {
    AnimationClip* nextClip;
    float transitionTime;
    AnimationClipCondition condition;
};

enum class AnimationClipComponentModificationType {
    FLOAT,
    INT,
    BOOL,
    POINTER
};

struct AnimationClipComponentModification {
    float timestampSeconds;
    FieldDescription* fieldToModify;
};

class AnimationClip {
public:
    AnimationClip();
    ~AnimationClip();

    void update(float dt);
    AnimationClipTransition transition;

    float durationSeconds; // do I need this?

    // TODO: needs some kind of reference to the component it wants to change
    // Also needs to keep track of which sprites to use, but could also just be
    // values to move e.g. a transform position
    Component* componentToModify;
    std::vector<AnimationClipComponentModification> modifications;
};

