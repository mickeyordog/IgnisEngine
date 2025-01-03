#pragma once

#include "texture.h"
#include "shader.h"
#include "componentVisual.h"
#include "model.h"

// TODO: rename to ModelRenderer?
// should prob have a sharedptr to model
class MeshRenderer : public ComponentVisual {
public:
    // MeshRenderer(Texture* texture, Shader* shader);
    MeshRenderer(Model* model, Shader* shader);
    virtual ~MeshRenderer();
    virtual void start() override;
    virtual void update(float dt) override;
    virtual void render() override;
    virtual Shader* getShader() override { return shader; };

    virtual enum ComponentType getType() override { return ComponentType::MESH_RENDERER; };
    virtual const std::vector<FieldDescription>& getFields() override { return fields; };

private:
    std::vector<FieldDescription> fields = { // TODO: replace with gltf
        { GET_NAME(model), FieldType::ASSET_POINTER_FIELD, &model, []() { }, ".glb" },
        { GET_NAME(shader), FieldType::ASSET_POINTER_FIELD, &shader, []() { }, ".shader" }
    };
    // GLuint VAO, VBO;
    // Texture* texture;
    // Shader* shader;
    Model* model;
    Shader* shader;
};