#pragma once

class Timer {
public:
    Timer();
    ~Timer();
    void reset();
    float read();
    float readAndReset();
    double readHiRes();
    double readHiResAndReset();

private:
    Uint64 startTicks;
    double ticksPerSec; 
};