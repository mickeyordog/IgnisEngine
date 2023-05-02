#pragma once
#include "sprite.h"
#include "window.h"

class GameObject {
public:
    GameObject(Sprite* sprite, int x, int y, int width, int height);
    ~GameObject();
    void update(float dt);
    void render(Renderer* renderer);

private:
    Sprite* sprite;
    int x;
    int y;
    int width;
    int height;
};