#pragma once

#include <vector>
#include <GL/glew.h>
#include "vec3.h"
#include "vec2.h"
#include "texture.h"
#include "shader.h"

struct Vertex {
    Vec3 Position;
    Vec3 Normal;
    Vec2 TexCoords;
};

class Mesh {
public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
         std::vector<Texture*> textures);
    ~Mesh();
    //Mesh(const Mesh&) = delete;
    //Mesh& operator=(const Mesh&) = delete;
    //Mesh(Mesh&&) = default;
    //Mesh& operator=(Mesh&&) = default;

    void render(const Shader& shader); // this shader should maybe come from material instead

private:
    GLuint VAO, VBO, EBO;
    std::vector<Vertex> vertices; // I don't really need to keep these around after sending to gpu, just need # of indices
    std::vector<unsigned int> indices;
    std::vector<Texture*> textures; // TODO: ideally these are shared ptrs
    void setupMesh();
};