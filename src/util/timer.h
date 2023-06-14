#pragma once

class Timer {
public:
    Timer();
    ~Timer();
    void reset();
    float read();
    float readAndReset();

private:
    Uint64 startTicks;
    float ticksPerSec; 
};