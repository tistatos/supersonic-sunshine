
#ifndef __MESH_H__
#define __MESH_H__

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "shader.h"


struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 tangent;
		glm::vec3 bitangent;
		glm::vec2 textureCoordinates;
};

struct Textures {
	GLuint diffuse;
	GLuint normal;
	GLuint roughness;
};


class Mesh {
public:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	Textures textures;
	Shader* shader;

	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices);
	void setModelMatrix(glm::mat4 matrix);
	void setRoughness(float roughness) { this->roughness = roughness; }

	void draw();

	glm::mat4 model;
private:
	float roughness = 0.25f;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
	GLuint vao, vbo, ibo;
	void setupMesh();
};

#endif
