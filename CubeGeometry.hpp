#pragma once

#include <glad/glad.h>
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Vertex.hpp"

class CubeGeometry {
public:
	CubeGeometry();
	~CubeGeometry();
	void init();
	void bind() const;
	void drawInstanced(const std::vector<glm::mat4>& models) const;
	void draw() const;

private:
	GLuint VAO, VBO, EBO;
	GLuint instanceVBO;

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

};