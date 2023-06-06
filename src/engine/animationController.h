#pragma once

#include "animationClip.h"
#include <vector>

class AnimationController {
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
    std::vector<AnimationClipVariable> variables;
};