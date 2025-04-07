#include "perlin.hpp"
#include <cmath>
#include <random>

PerlinNoise::PerlinNoise(std::array<int, 2> gridSize, std::array<int, 2> meshSize): 
    gridSize(gridSize), meshSize(meshSize) {
    generateGradients();
    generateNoiseGrid();
}

Vec2 PerlinNoise::randomUnitVector() const {
    static std::mt19937 gen(std::random_device{}());
    static std::uniform_real_distribution<float> dist(0.0f, 2.0f * PI);
    float angle = dist(gen);
    return { std::cos(angle), std::sin(angle) };
}

void PerlinNoise::generateGradients() {
    int rows = std::ceil(static_cast<float>(gridSize[1]) / meshSize[1]) + 1;
    int cols = std::ceil(static_cast<float>(gridSize[0]) / meshSize[0]) + 1;

    gradGrid.resize(rows, std::vector<Vec2>(cols));
    for (auto& row : gradGrid) {
        for (auto& g : row) {
            g = randomUnitVector();
        }
    }
}

void PerlinNoise::generateNoiseGrid() {
    noiseGrid.resize(gridSize[1], std::vector<float>(gridSize[0]));
    for (int y = 0; y < gridSize[1]; ++y) {
        for (int x = 0; x < gridSize[0]; ++x) {
            noiseGrid[y][x] = perlinAt(x, y);
        }
    }
}

float PerlinNoise::perlinAt(int x, int y) const {
    int x0 = x / meshSize[0];
    int y0 = y / meshSize[1];
    int x1 = x0 + 1;
    int y1 = y0 + 1;

    float dx = (x - x0 * meshSize[0]) / static_cast<float>(meshSize[0]);
    float dy = (y - y0 * meshSize[1]) / static_cast<float>(meshSize[1]);

    Vec2 p = { dx, dy };

    Vec2 g00 = gradGrid[y0][x0];
    Vec2 g10 = gradGrid[y0][x1];
    Vec2 g01 = gradGrid[y1][x0];
    Vec2 g11 = gradGrid[y1][x1];

    Vec2 d00 = { dx, dy };
    Vec2 d10 = { dx - 1, dy };
    Vec2 d01 = { dx, dy - 1 };
    Vec2 d11 = { dx - 1, dy - 1 };

    float dot00 = g00.dot(d00);
    float dot10 = g10.dot(d10);
    float dot01 = g01.dot(d01);
    float dot11 = g11.dot(d11);

    float ix0 = interpolate(dot00, dot10, dx);
    float ix1 = interpolate(dot01, dot11, dx);
    return interpolate(ix0, ix1, dy);
}

float PerlinNoise::getValue(int x, int y) const {
    return noiseGrid[y][x];
}

const std::vector<std::vector<float>>& PerlinNoise::getGrid() const {
    return noiseGrid;
}
