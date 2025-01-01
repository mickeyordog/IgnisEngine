#pragma once

#include "SDLContext.h"
#include "GLContext.h"

class DearImGuiContext
{
public:
    DearImGuiContext(SDLContext *sdlContext, GLContext *glContext);
    ~DearImGuiContext();

    void newFrame();
    void render();

    ImGuiIO& getIO();

private:
    void setStyling();
};