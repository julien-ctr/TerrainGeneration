#include "TerrainGenerator.hpp"
#include <iostream>


TerrainGenerator::TerrainGenerator() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
}

TerrainGenerator::~TerrainGenerator() {
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
};

void TerrainGenerator::genHeightmap(int width, int height, Parameters& parameters) {
	std::array<int, 2> size = { width, height };
	std::array<int, 2> mesh_size = { parameters.meshW, parameters.meshH };

	heightmap = octavesHeightmap(size, mesh_size, parameters.octaves, parameters.persistence, parameters.lacunarity);
	scaleMap(heightmap, 0, parameters.scale, width, height);
}