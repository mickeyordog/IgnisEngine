#pragma once

#include "texture.h"
#include "shader.h"
#include "componentVisual.h"

// TODO: allow instancing?
class SpriteRenderer : public ComponentVisual {
public:
    SpriteRenderer(const Texture* texture, Shader* shader);
    ~SpriteRenderer();
    virtual void start() override;
    virtual void update(float dt) override;
    virtual void render() override;
    virtual Shader& getShader() override { return *shader; };

    virtual enum ComponentType getType() override { return ComponentType::SPRITE_RENDERER; };
    virtual std::vector<FieldDescription>& getFields() override { return fields; };

private:
    std::vector<FieldDescription> fields = { };

    GLuint VAO, VBO, EBO;
    const Texture* texture;
    Shader* shader;
};