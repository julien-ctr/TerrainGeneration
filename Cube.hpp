#pragma once
#include "CubeGeometry.hpp"
#include "shaders.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Cube {
public:
	Cube(float x, float y, float z, float scale = 1.0f);
	static void setGeometry(CubeGeometry* geometry);

	void draw(Shaders& shader) const;

private:
	float x, y, z, scale;
	static CubeGeometry* geometry;
};