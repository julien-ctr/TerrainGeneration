#pragma once
#include "Parameters.hpp"
#include <glad/glad.h>
#include <vector>
#include <array>
#include "perlin.hpp"
#include "shaders.hpp"
#include "Camera.hpp"

class TerrainGenerator {
public:
	TerrainGenerator();
	~TerrainGenerator();

	void genHeightmap(int width, int height, Parameters& parameters);
	virtual void generateTerrain(Parameters& parameters) = 0;
	virtual void render(Camera& camera) = 0;
	virtual void setShader(std::shared_ptr<Shaders> shader) = 0;
	void bind() const { glBindVertexArray(VAO); };

protected:
	GLuint VAO, VBO, EBO;
	std::vector<std::vector<float>> heightmap;
};