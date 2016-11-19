/**
* @file mesh.cpp
* @author Erik Sandrén
* @date 13-11-2016
* @brief Mesh class
*/
#include "mesh.h"
#include <iostream>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices) {
		this->vertices = vertices;
		this->indices = indices;
		this->setupMesh();
		this->model = glm::mat4(1.0f);

		this->diffuseColor = glm::vec3(1.f,1.f,1.f);
		this->specularColor = glm::vec3(1.f,1.f,1.f);
}
void Mesh::setModelMatrix(glm::mat4 matrix) {
	this->model = matrix;

}

void Mesh::draw() {
	glUseProgram(*this->shader);
	glUniform1i(glGetUniformLocation(*this->shader, "tex"), 0 );
	glUniform1i(glGetUniformLocation(*this->shader, "ampTex"), 1);

	glUniform1f(glGetUniformLocation(*this->shader, "roughness"), roughness );
	glUniformMatrix4fv(glGetUniformLocation(*this->shader, "m"),
			1, GL_FALSE, glm::value_ptr(this->model));

	glUniform3fv(glGetUniformLocation(*shader, "diffuseColor"), 1,glm::value_ptr(diffuseColor));
	glUniform3fv(glGetUniformLocation(*shader, "specularColor"), 1,glm::value_ptr(specularColor));

	glBindVertexArray(this->vao);
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glUseProgram(0);
}

void Mesh::setupMesh() {
	// Create buffers/arrays
	glGenVertexArrays(1, &this->vao);
	glGenBuffers(1, &this->vbo);
	glGenBuffers(1, &this->ibo);

	glBindVertexArray(this->vao);

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

	// Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

	// Vertex Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));

	// Vertex Texture Coordinates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, textureCoordinates));
	glBindVertexArray(0);
}
