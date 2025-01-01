#pragma once

#include "texture.h"

struct Material {
    Texture* diffuse;
    Texture* specular;
    float shininess;
};