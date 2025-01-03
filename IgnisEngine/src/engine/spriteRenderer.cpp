#include "spriteRenderer.h"
#include <GL/glew.h>
#include <iostream>

SpriteRenderer::SpriteRenderer(Texture* texture, Shader* shader) : texture(texture), shader(shader)
{
    fields.insert(fields.begin(), ComponentVisual::getFields().begin(), ComponentVisual::getFields().end());

    // x, y, z, u, v
    const float vertices[] = {
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f,   // top right
        0.5f, -0.5f, 0.0f, 1.0f, 1.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom left
        -0.5f, 0.5f, 0.0f, 0.0f, 0.0f   // top left
    };
    const unsigned int indices[] = {
        0, 3, 1, 
        1, 3, 2
    };

    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);

    glGenBuffers(1, &this->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &this->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

SpriteRenderer::~SpriteRenderer()
{
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
    glDeleteBuffers(1, &this->EBO);
}

void SpriteRenderer::start()
{

}

void SpriteRenderer::update(float dt)
{

}

void SpriteRenderer::render()
{
    this->texture->bind();
    this->shader->use();

    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
