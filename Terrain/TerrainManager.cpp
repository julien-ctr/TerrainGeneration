#include "TerrainManager.hpp"

TerrainManager::TerrainManager() {
	currentMode = std::make_unique<ClassicTerrain>();
	init();
}

void TerrainManager::init() {
	classicShader = std::make_shared<Shaders>("Shaders/classic.vs", "Shaders/classic.fs");
	voxelShader = std::make_shared<Shaders>("Shaders/vertexShader.vs", "Shaders/fragmentShader.fs");

	dynamic_cast<ClassicTerrain*>(currentMode.get())->setShader(classicShader);
}

void TerrainManager::switchMode() {
	isVoxel = !isVoxel;

	if (isVoxel) {
		auto voxel = std::make_unique<VoxelTerrain>();
		voxel->setShader(voxelShader);
		currentMode = std::move(voxel);
	}
	else {
		auto classic = std::make_unique<ClassicTerrain>();
		classic->setShader(classicShader);
		currentMode = std::move(classic);
	}
}

void TerrainManager::generate(Parameters& parameters) {
	currentMode->generateTerrain(parameters);
}

void TerrainManager::render(Camera& camera) {
	currentMode->bind();
	currentMode->render(camera);
}