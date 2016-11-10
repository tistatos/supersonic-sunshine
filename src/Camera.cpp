//
//  Camera.cpp
//
//  Created by Pontus Orraryd on 2015-07-28.
//  Copyright (c) 2015 Pontus Orraryd. All rights reserved.
//

#include "Camera.h"


Camera::Camera(int width, int height)
{
	mWidth = width;
	mHeight = height;
	float aspect = (float)width/height;
	matrices.projection = glm::perspective((float)M_PI/3.0f, aspect, 0.001f, 1000.0f);
	matrices.view = glm::lookAt(
				glm::vec3(0.f, 1.5f, 6.f), //position
				glm::vec3(0.f, 0.5f, 0.f),  // look at
				glm::vec3(0.f, 1.f, 0.f) // up-vector
				);

	//Create uniform buffer:
	glGenBuffers(1, &uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(camera_matrices), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	update();
}

void Camera::update(){
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	GLvoid* p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
	assert(p != NULL);
	memcpy(p, &this->matrices, sizeof(camera_matrices));
	glUnmapBuffer(GL_UNIFORM_BUFFER);

	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, sizeof(camera_matrices));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
