
#ifndef __MESH_H__
#define __MESH_H__

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "shader.h"


struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 textureCoordinates;
};

class Mesh {
public:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	Shader* shader;


	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices);
	void setModelMatrix(glm::mat4 matrix);
	void setRoughness(float roughness) { this->roughness = roughness; }

	void draw();

	glm::mat4 model;
private:
	float roughness = 0.25f;
	GLuint vao, vbo, ibo;
	void setupMesh();
};

#endif
