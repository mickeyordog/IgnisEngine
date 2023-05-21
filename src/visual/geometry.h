#pragma once

#include <GL/glew.h>

class Geometry {
public:
    Geometry();
    ~Geometry();
    void render();

private:
    GLuint VAO, VBO, EBO;
};