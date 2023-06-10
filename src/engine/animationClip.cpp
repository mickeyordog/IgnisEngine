#include "animationClip.h"

AnimationClip::AnimationClip()
{
    
}

AnimationClip::~AnimationClip()
{
    
}

void AnimationClip::update(float dt)
{
    timeInClip += dt;
    if (timeInClip > durationSeconds)
    {
        incrementClip();
    }

}

void AnimationClip::incrementClip()
{
    modificationIndex++;
    if (modificationIndex >= modifications.size())
    {
        modificationIndex = 0;
        timeInClip -= durationSeconds;
    }
}

void AnimationClip::applyModifications()
{
    Keyframe& modification = modifications[modificationIndex];
    switch (modification.type)
    {
        case KeyframeModificationType::POINTER:

            break;
        default:
            break;
    }
}
