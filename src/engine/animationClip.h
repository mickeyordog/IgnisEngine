#pragma once

#include "component.h"

enum class KeyframeModificationType {
    FLOAT,
    INT,
    BOOL,
    POINTER
};

struct Keyframe {
public:
    float timestampSeconds;
    KeyframeModificationType type;
    FieldDescription& fieldToModify;
    Keyframe(float timestampSeconds, KeyframeModificationType type, FieldDescription& fieldToModify) :
        timestampSeconds(timestampSeconds), type(type), fieldToModify(fieldToModify) { }
    virtual ~Keyframe() { }

    virtual void applyModification() = 0;
};

#include "assetManager.h"
struct SpriteKeyframe : public Keyframe {
public:
    IgnisGUID textureGuid;

    virtual void applyModification() override {
        *(Asset**)fieldToModify.ptr = AssetManager::loadOrGetAsset(textureGuid);

    }
};

class AnimationClip {
public:
    AnimationClip();
    ~AnimationClip();

    void update(float dt);

private:
    void incrementClip();
    void applyModifications();
    // TODO: needs some kind of reference to the component it wants to change
    // Also needs to keep track of which sprites to use, but could also just be
    // values to move e.g. a transform position
    Component* componentToModify;
    std::vector<Keyframe> modifications; // this could technically be optimized into a unique ptr to new int[]
    int modificationIndex = 0;

    float timeInClip = 0.0f;
    float durationSeconds; // TODO: this should be calculated from the modifications at init
};

