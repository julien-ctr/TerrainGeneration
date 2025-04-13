#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "Camera.hpp"
#include "Terrain/TerrainManager.hpp"
#include "Parameters.hpp"

class UI {
public:
	UI();

	void init(GLFWwindow* window);
	void render(Parameters& parameters, GLFWwindow* window, Camera& camera, unsigned int textureColorbuffer, TerrainManager& terrainGen, Stats& stats);

private:

};