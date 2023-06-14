#include <iostream>
#include "renderTexture.h"
#include "GLContext.h"

RenderTexture::RenderTexture(int width, int height) : width(width), height(height)
{   glCheckError();
    glGenFramebuffers(1, &this->frameBuffer);glCheckError();
    glBindFramebuffer(GL_FRAMEBUFFER, this->frameBuffer);glCheckError();

    glGenTextures(1, &this->texture);
    glBindTexture(GL_TEXTURE_2D, this->texture);glCheckError();
    // TODO: pass in size
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);glCheckError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);glCheckError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);glCheckError();
    // TODO: might need to call glviewport? Not sure how that works

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->texture, 0);glCheckError();

    glGenRenderbuffers(1, &this->renderBuffer);glCheckError();
    glBindRenderbuffer(GL_RENDERBUFFER, this->renderBuffer);glCheckError();
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);glCheckError();
    glBindRenderbuffer(GL_RENDERBUFFER, 0);glCheckError();
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);glCheckError();

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

RenderTexture::~RenderTexture()
{
    glDeleteTextures(1, &this->texture);glCheckError();
    glDeleteRenderbuffers(1, &this->renderBuffer);glCheckError();
    glDeleteFramebuffers(1, &this->frameBuffer);glCheckError();
    
}

void RenderTexture::bind()
{
    glCheckError();
    glBindFramebuffer(GL_FRAMEBUFFER, this->frameBuffer);glCheckError();
}

void RenderTexture::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);glCheckError();
}
