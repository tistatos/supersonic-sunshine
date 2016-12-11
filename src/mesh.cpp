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
}

void Mesh::setModelMatrix(glm::mat4 matrix) {
	this->model = matrix;
}

void Mesh::draw() {
	glUseProgram(*this->shader);
	glUniform1i(glGetUniformLocation(*this->shader, "ltcTexture"), 0 );
	glUniform1i(glGetUniformLocation(*this->shader, "ltcAmplitude"), 1);

	glUniform1f(glGetUniformLocation(*this->shader, "roughness"), roughness );
	glUniformMatrix4fv(glGetUniformLocation(*this->shader, "m"),
			1, GL_FALSE, glm::value_ptr(this->model));

	if (textures.normal > 0){
		glActiveTexture(GL_TEXTURE2);
		glUniform1i(glGetUniformLocation(*shader,"normalMap"), 2);
		glBindTexture(GL_TEXTURE_2D, textures.normal);
	}
	if (textures.roughness > 0){
		glActiveTexture(GL_TEXTURE3);
		glUniform1i(glGetUniformLocation(*shader,"roughnessMap"), 3);
		glBindTexture(GL_TEXTURE_2D, textures.roughness);
	}

	if (textures.diffuse > 0){
		glActiveTexture(GL_TEXTURE4);
		glUniform1i(glGetUniformLocation(*shader,"diffuseMap"), 4);
		glBindTexture(GL_TEXTURE_2D, textures.diffuse);
	}
	glActiveTexture(GL_TEXTURE0);

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

	//vertex bitangent
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, bitangent));

	//Vertex tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, tangent));

	// Vertex Texture Coordinates
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, textureCoordinates));

	glBindVertexArray(0);
}
