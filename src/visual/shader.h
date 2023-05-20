#pragma once

class Shader
{
public:
    Shader(const char *vertexPath, const char *fragmentPath);
    ~Shader();
    void use();
    // TODO: uniform helpers
    // void setFloat(const std::string &name, float value) const;

private:
    unsigned int id;
};