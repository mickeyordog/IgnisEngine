#pragma once
#include <random>
#include <limits>

class RandomNumberGenerator {
public:
    static long long getRandomInteger() {
        static std::random_device rd;
        static std::mt19937 mt(rd());
        static std::uniform_int_distribution<long long> dist(0, LLONG_MAX);
        return dist(mt);
    }
};