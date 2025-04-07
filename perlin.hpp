#pragma once
#include "utils.hpp"
#include <vector>
#include <array>

#define PI 3.1415926535

struct Vec2 {
    float x, y;
    Vec2 operator-(const Vec2& other) const { return { x - other.x, y - other.y }; }
    Vec2 operator+(const Vec2& other) const { return { x + other.x, y + other.y }; }
    float dot(const Vec2& other) const { return x * other.x + y * other.y; }
};

class PerlinNoise {
private:
    std::array<int, 2> gridSize;
    std::array<int, 2> meshSize;
    std::vector<std::vector<float>> noiseGrid;
    std::vector<std::vector<Vec2>> gradGrid;

    void generateGradients();
    void generateNoiseGrid();
    Vec2 randomUnitVector() const;
    float perlinAt(int x, int y) const;

public:
    PerlinNoise(std::array<int, 2> gridSize, std::array<int, 2> meshSize);
    float getValue(int x, int y) const;
    const std::vector<std::vector<float>>& getGrid() const;
};
