#pragma once

#include "texture.h"

// this will be the result after you slice the texture
// sprite renderer will show one of these, display it based on texture and uvs
// animator can swap them out
class Sprite {
public:
    Texture* texture;
    float u, v;
};