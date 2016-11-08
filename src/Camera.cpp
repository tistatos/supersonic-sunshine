//
//  Camera.cpp
//
//  Created by Pontus Orraryd on 2015-07-28.
//  Copyright (c) 2015 Pontus Orraryd. All rights reserved.
//

#include "Camera.h"

Camera::Camera(int width, int height)
{
	screenWidth = width;
	screenHeight = height;
	float aspect = (float)width/height;
	mProj = glm::perspective((float)M_PI/3.0f, (float)screenWidth/screenHeight, 0.001f, 1000.0f);
	mView = glm::lookAt(
				glm::vec3(0.f, 1.5f, 6.f), //position
				glm::vec3(0.f, 0.5f, 0.f),  // look at
				glm::vec3(0.f, 1.f, 0.f) // up-vector
				);

	//Create uniform buffer:
	glGenBuffers(1, &uboMatrices);

	update();
}


void Camera::updateUniform(){
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(mView));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Camera::setProjectionMatrix(glm::mat4 proj){
	mProj = proj;
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(mProj));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Camera::update(){

	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));

	//Put in the Projection matrix.

	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(mProj));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	//put in View Matrix
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(mView));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

}
