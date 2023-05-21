#pragma once

#include "SDLContext.h"

// TODO: rename to GLContext
class GLContext {
public:
    GLContext(SDLContext* window);
    ~GLContext();

    void clear();
};