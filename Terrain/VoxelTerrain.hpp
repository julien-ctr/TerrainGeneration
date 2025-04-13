#pragma once

#include <glad/glad.h>

#include <vector>
#include <array>
#include "perlin.hpp"
#include "Camera.hpp"
#include "Parameters.hpp"
#include "TerrainGenerator.hpp"
#include "CubeGeometry.hpp"

class VoxelTerrain : public TerrainGenerator {
public:
    VoxelTerrain();
    void generateTerrain(Parameters& parameters) override;
    int getIndicesSize();
    void render(Camera& camera) override;
    void setShader(std::shared_ptr<Shaders> shader) override;


private:
    std::vector<float> vertices;
    std::vector<uint32_t> indices;
    CubeGeometry cubeGeo;
    std::vector<glm::mat4> cubeModels;
    std::shared_ptr<Shaders> shader;
};