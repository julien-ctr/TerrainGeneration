#include "TerrainGenerator.hpp"

TerrainGenerator::TerrainGenerator(){}

void TerrainGenerator::generateTerrain(Parameters& parameters) {
    getHeightmap(100, 100, parameters);
    getVerticesAndIndices();
}

void TerrainGenerator::getHeightmap(int width, int height, Parameters& parameters) {
    std::array<int, 2> size = { width, height };
    std::array<int, 2> mesh_size = { parameters.meshW, parameters.meshH };

    heightmap = octavesHeightmap(size, mesh_size, parameters.octaves, parameters.persistence, parameters.lacunarity);
    scaleMap(heightmap, 0, parameters.scale, width, height);
}

void TerrainGenerator::getVerticesAndIndices() {
    vertices.clear();
    indices.clear();

    int mapwidth = heightmap[0].size();
    int mapdepth = heightmap.size();

    for (int z = 0; z < mapdepth; ++z) {
        for (int x = 0; x < mapwidth; ++x) {
            float posX = static_cast<float>(x);
            float posY = heightmap[z][x];
            float posZ = static_cast<float>(z);

            float texX = static_cast<float>(x) / (mapwidth - 1);
            float texY = static_cast<float>(z) / (mapdepth - 1);

            vertices.push_back(posX);
            vertices.push_back(posY);
            vertices.push_back(posZ);
            vertices.push_back(texX);
            vertices.push_back(texY);
        }
    }

    for (unsigned int z = 0; z < mapdepth - 1; ++z) {
        for (unsigned int x = 0; x < mapwidth - 1; ++x) {
            unsigned int topLeft = z * mapwidth + x;
            unsigned int topRight = topLeft + 1;
            unsigned int bottomLeft = (z + 1) * mapwidth + x;
            unsigned int bottomRight = bottomLeft + 1;

            if ((x + z) % 2 == 0) {
                indices.insert(indices.end(), { topLeft, bottomLeft, bottomRight });
                indices.insert(indices.end(), { topLeft, topRight, bottomRight });
            }
            else {
                indices.insert(indices.end(), { topLeft, topRight, bottomLeft });
                indices.insert(indices.end(), { topRight, bottomRight, bottomLeft });
            }
        }
    }
}

void TerrainGenerator::loadHeightmapToBuffer(unsigned int VBO, unsigned int EBO) {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

std::vector<std::vector<float>>& TerrainGenerator::getHeightmapData() {
    return heightmap;
}

int TerrainGenerator::getIndicesSize() {
    return indices.size();
}