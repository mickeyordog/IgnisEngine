#include <iostream>
#include "renderTexture.h"

RenderTexture::RenderTexture(int width, int height) : width(width), height(height)
{
    glGenFramebuffers(1, &this->frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, this->frameBuffer);

    // TODO: prob need to attach a depth texture with same process
    glGenTextures(1, &this->texture);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    // TODO: pass in size
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // TODO: might need to call glviewport? Not sure how that works

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->texture, 0);

    glGenRenderbuffers(1, &this->renderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, this->renderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

RenderTexture::~RenderTexture()
{
    glDeleteTextures(1, &this->texture);
    glDeleteRenderbuffers(1, &this->renderBuffer);
    glDeleteFramebuffers(1, &this->frameBuffer);
}

void RenderTexture::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, this->frameBuffer);
}

void RenderTexture::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
