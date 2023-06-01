#pragma once

// #include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
    Shader(const char *vertexPath, const char *fragmentPath);
    ~Shader();
    void use() const;
    // TODO: prob use class wrapper for param
    void setUniform(const char* name, glm::mat4& value);
    // TODO: uniform helpers
    // void setFloat(const std::string &name, float value) const;

    unsigned int id;
};