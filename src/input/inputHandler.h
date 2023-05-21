#pragma once

// Singleton implementation from https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
class InputHandler
{
public:
    static InputHandler& getInstance();
    InputHandler(InputHandler const&) = delete;
    void operator=(InputHandler const&) = delete;

    void updateKeys();

private:
    InputHandler() {}
};