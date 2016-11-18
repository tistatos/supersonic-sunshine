#include "arealight.h"

AreaLight::AreaLight(float x, float y, float intensity){

	mLightMesh = Util::createPlaneMeshPointer(x,y);
	mProperties.points[0] = glm::vec3(x, 0.f, y);
	mProperties.points[1] = glm::vec3(x, 0.f, -y);
	mProperties.points[2] = glm::vec3(-x, 0.f, -y);
	mProperties.points[3] = glm::vec3(-x, 0.f, y);
	mProperties.intensity = intensity;

	mProperties.color = glm::vec3(1.f,0.6f,0.6f);
	mProperties.model = glm::mat4(1.0f);

	mProperties.model  = glm::rotate(mProperties.model, (float)(M_PI/2.0f), glm::vec3(1.0f,0.0f,0.0f));
	mProperties.model  = glm::translate(mProperties.model, glm::vec3(0.0f, 0.0f, -1.0f));
	mLightMesh->setModelMatrix(mProperties.model);


	Shader* light = new Shader("../shaders/simple.vert","../shaders/light.frag");
	mLightMesh->shader = light;
}

void AreaLight::use(Shader &shader){

	glUseProgram(shader);
	GLint pointsLoc = glGetUniformLocation(shader, "arealight.points");
	GLint modelLoc = glGetUniformLocation(shader, "arealight.M");
	GLint intensityLoc = glGetUniformLocation(shader, "arealight.intensity");
	GLint colorLoc = glGetUniformLocation(shader, "arealight.color");

	glUniform3fv(pointsLoc, 4, glm::value_ptr(mProperties.points[0]));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mProperties.model));
	glUniform1f(intensityLoc, mProperties.intensity);
	glUniform3fv(colorLoc,1, glm::value_ptr(mProperties.color));

	glUseProgram(0);
}

void AreaLight::draw(){
	//mLightMesh.setModelMatrix(mProperties.model);
	mLightMesh->draw();
}
