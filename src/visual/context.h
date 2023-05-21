#pragma once

#include "window.h"

// TODO: rename to GLContext
class Context {
public:
    Context(Window* window);
    ~Context();

    void clear();
};