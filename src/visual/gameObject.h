#pragma once
#include "sprite.h"
#include "window.h"

class GameObject {
public:
    GameObject(Sprite* sprite, float x, float y, int width, int height);
    ~GameObject();
    void update(float dt);
    void render(Renderer* renderer);

private:
    Sprite* sprite;
    float x;
    float y;
    int width;
    int height;
};