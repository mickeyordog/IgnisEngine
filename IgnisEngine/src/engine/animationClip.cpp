#include "animationClip.h"

AnimationClip::AnimationClip(GameObject* gameObject, std::vector<Keyframe> keyframes, ComponentType componentTypeToModify, std::string fieldToModifyName)
{
    // TODO: clip data will contain a component type ID and a param field name to modify, alon with
    // a list of values to change to at each time (e.g. guid for new sprite texture)
    // upong creation, clip will iterate over game object components to find one with right type,
    // then over fields to find correct field. Then it stores the field and can use it for 
    // rest of game time
    // Each AnimController can prob create its own anim clips, each one should have its own object but can be made
    // from file asset blueprint (assetmanager will prob still need to give blueprint though? Then maybe the actual object
    // will just contain the specific timing of that object, or maybe that whole thing could be controlled by the anim controller,
    // then just pass relevant info into the shared animation clips and controllers and they can change the values themselves)
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
    if (modificationIndex >= keyframes.size())
    {
        modificationIndex = 0;
        timeInClip -= durationSeconds;
    }
}

void AnimationClip::applyModifications()
{
    Keyframe& modification = keyframes[modificationIndex];
    switch (modification.type)
    {
        case KeyframeModificationType::POINTER:

            break;
        default:
            break;
    }
}
