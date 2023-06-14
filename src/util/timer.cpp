#include <SDL.h>
#include "timer.h"

Timer::Timer()
{
    this->ticksPerSec = (float)SDL_GetPerformanceFrequency();
    reset();
}

Timer::~Timer()
{
    
}

void Timer::reset()
{
    this->startTicks = SDL_GetPerformanceCounter();
}

float Timer::read()
{
    Uint64 currentTicks = SDL_GetPerformanceCounter();
    return (currentTicks - this->startTicks) / this->ticksPerSec;
}

float Timer::readAndReset()
{
    float time = read();
    reset();
    return time;
}
