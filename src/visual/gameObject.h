#pragma once
#include "sprite.h"
#include "window.h"

class GameObject {
public:
    GameObject(Sprite* sprite);
    ~GameObject();
    void update(float dt);
    void render(Window* window);

private:
    Sprite* sprite;
};