#pragma once

#include <glad/glad.h>

#include <vector>
#include <array>
#include "perlin.hpp"
#include "Camera.hpp"
#include "Parameters.hpp"
#include "TerrainGenerator.hpp"
#include "Vertex.hpp"

class ClassicTerrain : public TerrainGenerator {
public:
    ClassicTerrain();
    void generateTerrain(Parameters& parameters) override;
    void getHeightmap(int width, int height, Parameters& parameters);
    void getVerticesAndIndices();
    void loadHeightmapToBuffer();

    std::vector<std::vector<float>>& getHeightmapData();
    int getIndicesSize();
    void render(Camera& camera) override;
    void setShader(std::shared_ptr<Shaders> shader) override;

private:
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::shared_ptr<Shaders> shader;
};