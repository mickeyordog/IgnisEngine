#pragma once

#include "texture.h"
#include "shader.h"
#include "componentVisual.h"

// TODO: allow instancing?
class SpriteRenderer : public ComponentVisual {
public:
    SpriteRenderer(const Texture* texture, Shader* shader);
    ~SpriteRenderer();
    virtual void start();
    virtual void update(float dt);
    virtual void render();
    virtual Shader& getShader() { return *shader; };

private:
    GLuint VAO, VBO, EBO;
    const Texture* texture;
    Shader* shader;
};