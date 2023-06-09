#pragma once

// #include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "asset.h"

class Shader : public Asset
{
public:
    Shader(const char* vertexCode, const char* fragmentCode);
    ~Shader();
    void use() const;
    // TODO: prob use class wrapper for param
    void setUniform(const char* name, const glm::mat4& value);
    // TODO: uniform helpers
    // void setFloat(const std::string &name, float value) const;

    unsigned int id;
};