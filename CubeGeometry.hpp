#pragma once

#include <glad/glad.h>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Vertex {
	glm::vec3 position;
	glm::vec2 texture;
	glm::vec3 normal;
};

class CubeGeometry {
public:
	CubeGeometry();
	~CubeGeometry();
	void init();
	void bind() const;
	void draw() const;

private:
	GLuint VAO, VBO, EBO;
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

};