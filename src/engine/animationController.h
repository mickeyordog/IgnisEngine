#pragma once

#include "animationClip.h"
#include <vector>

enum class AnimationControllerVariableType {
    INT,
    FLOAT,
    BOOL,
    TRIGGER
};

struct AnimationControllerVariable {
    AnimationControllerVariableType type;
    char* name;
    union {
        float floatValue;
        int intValue;
        bool boolValue;
    };
};

enum class AnimationTransitionConditionType {
    EQUAL,
    NOT_EQUAL,
    LESS_THAN,
    GREATER_THAN,
    LESS_THAN_OR_EQUAL,
    GREATER_THAN_OR_EQUAL
};

struct AnimationTransitionCondition {
    AnimationControllerVariable variable;
    AnimationTransitionConditionType type;
};

struct AnimationClipTransition {
    AnimationClip* nextClip;
    float transitionTime;
    AnimationTransitionCondition condition;
};

class AnimationController : public Asset {
public:
    AnimationController();
    ~AnimationController();

    void update(float dt);
    void addClip(AnimationClip* clip) { clips.push_back(clip); };
    void setVariable(char* name, float value);
    void setVariable(char* name, int value);
    void setVariable(char* name, bool value);

    AnimationClip* startingClip;

private:
    std::vector<AnimationClip*> clips;
    std::vector<AnimationControllerVariable> variables;
};