#pragma once
#include <memory>
#include "TerrainGenerator.hpp"
#include "ClassicTerrain.hpp"
#include "VoxelTerrain.hpp"
#include "shaders.hpp"

class TerrainManager {
private:
	std::unique_ptr<TerrainGenerator> currentMode;
	bool isVoxel = false;

	std::shared_ptr<Shaders> classicShader;
	std::shared_ptr<Shaders> voxelShader;

public:
	TerrainManager();
	void init();
	void switchMode();
	void generate(Parameters& parameters);
	void render(Camera& camera);
};