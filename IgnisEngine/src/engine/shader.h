#pragma once

// #include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "asset.h"
#include "vec3.h"

class Shader : public Asset {
public:
    Shader(const char* vertexCode, const char* fragmentCode);
    virtual ~Shader() override;
    virtual Asset* clone() override { return new Shader(*this); }
    virtual AssetType getAssetType() const override { return AssetType::SHADER; }
    void use() const;
    // TODO: prob use class wrapper for param
    void setUniform(const char* name, const glm::mat4& value) const;
    // TODO: uniform helpers
    void setFloat(const char* name, float value) const;
    void setInt(const char* name, int value) const;
    void setVec3(const char* name, const Vec3& value) const;

    unsigned int getID() const { return id; }
    
private:
    unsigned int id;
};