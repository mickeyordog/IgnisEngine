#pragma once

#include <GL/glew.h>
#include "texture.h"
#include "shader.h"

// TODO: allow instancing?
class Geometry {
public:
    Geometry(const Texture* texture, const Shader* shader);
    ~Geometry();
    void render();

private:
    GLuint VAO, VBO, EBO;
    const Texture* texture;
    const Shader* shader;
};