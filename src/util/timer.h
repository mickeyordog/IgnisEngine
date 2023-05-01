#pragma once

class Timer {
public:
    Timer();
    ~Timer();
    void reset();
    float read();

private:
    Uint64 startTicks;
    float ticksPerSec; 
};