#pragma once

#include <glad/glad.h>

#include <vector>
#include <array>
#include "perlin.hpp"
#include "Camera.hpp"
#include "Parameters.hpp"

class TerrainGenerator {
public:
    TerrainGenerator();
    void generateTerrain(Parameters& parameters);
    void getHeightmap(int width, int height, Parameters& parameters);
    void getVerticesAndIndices();
    void loadHeightmapToBuffer(unsigned int VBO, unsigned int EBO);

    std::vector<std::vector<float>>& getHeightmapData();
    int getIndicesSize();

private:
    std::vector<std::vector<float>> heightmap;
    std::vector<float> vertices;
    std::vector<uint32_t> indices;
};