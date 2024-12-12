#pragma once

#include "texture.h"
#include "shader.h"
#include "componentVisual.h"

// TODO: allow gpu instancing?
class SpriteRenderer : public ComponentVisual {
public:
    SpriteRenderer(Texture* texture, Shader* shader);
    ~SpriteRenderer();
    virtual void start() override;
    virtual void update(float dt) override;
    virtual void render() override;
    virtual Shader& getShader() override { return *shader; };

    virtual enum ComponentType getType() override { return ComponentType::SPRITE_RENDERER; };
    virtual std::vector<FieldDescription>& getFields() override { return fields; };

private:
    // TODO: thinking about how I want to reference texture and shader. Should prob be using handle instead of pointer.
    // Not sure how I should serialize the reference though. Probably review game engine textbook because I think 
    // there was some discussion about how to save references in the context of going from scene to scene, so similar to here
    std::vector<FieldDescription> fields = {
        { GET_NAME(texture), FieldType::ASSET_POINTER_FIELD, &texture, [](){ }, ".png" },
        { GET_NAME(shader), FieldType::ASSET_POINTER_FIELD, &shader, []() { }, ".shader" }
    };

    GLuint VAO, VBO, EBO;
    Texture* texture;
    Shader* shader;
};