#pragma once

#include "windowContext.h"
#include "GLContext.h"

class DearImGuiContext
{
public:
    DearImGuiContext(WindowContext *sdlContext, GLContext *glContext);
    ~DearImGuiContext();

    void newFrame();
    void render();

    ImGuiIO& getIO();

private:
    void setStyling();
};