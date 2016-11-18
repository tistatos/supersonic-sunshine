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
	mFacing = glm::vec3(0.0f,0.f,-1.0);
	mPosition = glm::vec3(0,1.5,6.f);
	float aspect = (float)width/height;
	matrices.projection = glm::perspective((float)M_PI/3.0f, aspect, 0.001f, 1000.0f);
	matrices.view = glm::lookAt(
			mPosition,
			mPosition + mFacing,
			glm::vec3(0.0f, 1.0f,0.0f)
				);

	//Create uniform buffer:
	glGenBuffers(1, &uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(camera_matrices), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	update();
}

void Camera::update(){
	matrices.view = glm::lookAt(
			mPosition,
			mPosition + mFacing,
			glm::vec3(0.0f, 1.0f,0.0f)
	 );
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	GLvoid* p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
	assert(p != NULL);
	memcpy(p, &this->matrices, sizeof(camera_matrices));
	glUnmapBuffer(GL_UNIFORM_BUFFER);

	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, sizeof(camera_matrices));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
