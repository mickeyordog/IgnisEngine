#pragma once

class GameObject {
public:
    GameObject(float x, float y, int width, int height);
    ~GameObject();
    void update(float dt);
    void render();

private:
    float x;
    float y;
    int width;
    int height;
};