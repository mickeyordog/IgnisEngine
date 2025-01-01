#include <SDL.h>
#include "timer.h"

Timer::Timer()
{
    this->ticksPerSec = (double)SDL_GetPerformanceFrequency();
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
    return (float)readHiRes();
}

float Timer::readAndReset()
{
    return (float)readHiResAndReset();
}

double Timer::readHiRes() {
    Uint64 currentTicks = SDL_GetPerformanceCounter();
    return (currentTicks - this->startTicks) / this->ticksPerSec;
}

double Timer::readHiResAndReset() {
    double time = readHiRes();
    reset();
    return time;
}
