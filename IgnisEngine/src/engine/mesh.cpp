#include "mesh.h"
#include <iostream>
#include <string>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
     std::vector<Texture*> textures) : vertices(vertices), indices(indices), textures(textures)
{
    setupMesh();
}

Mesh::~Mesh() { 
    //glDeleteVertexArrays(1, &VAO);
    //glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);
}

void Mesh::render(const Shader& shader)
{ 
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // activate texture unit first
        // retrieve texture number (the N in diffuse_textureN)
        std::string name;
        std::string number;
        TextureType type = textures[i]->type;
        switch (type) {
            case TextureType::DIFFUSE:
                name = "texture_diffuse";
                number = std::to_string(diffuseNr++);
                break;
            case TextureType::SPECULAR:
                name = "texture_specular";
                number = std::to_string(specularNr++);
                break;
            default:
                break;
        }
        shader.setInt((/*"material." + */name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i]->textureHandle);
    }
    glActiveTexture(GL_TEXTURE0);
    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                 &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() *
                 sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, Normal));

    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);

    // print indices
    for (int i = 0; i < indices.size(); i++) {
        std::cout << "Index " << i << ": " << indices[i] << std::endl;
    }
     //print vertices
     for (int i = 0; i < vertices.size(); i++) {
         std::cout << "Vertex " << i << ": " << vertices[i].Position.x() << ", " << vertices[i].Position.y() << ", " << vertices[i].Position.z() << std::endl;
     }
    // print normals
    // for (int i = 0; i < vertices.size(); i++) {
    //     std::cout << "Normal " << i << ": " << vertices[i].Normal.x() << ", " << vertices[i].Normal.y() << ", " << vertices[i].Normal.z() << std::endl;
    // }
}
