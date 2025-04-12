#include "Cube.hpp"

CubeGeometry* Cube::geometry = nullptr;

Cube::Cube(float x, float y, float z, float scale) : x(x), y(y), z(z), scale(scale) {}

void Cube::setGeometry(CubeGeometry* geometry) {
	Cube::geometry = geometry;
}

void Cube::draw(Shaders& shader) const {
	glm::mat4 model = glm::translate(glm::mat4(1.0f), { x, y ,z });
	model = glm::scale(model, glm::vec3(scale));
	shader.setMat4("model", model);
	geometry->bind();
	geometry->draw();
}