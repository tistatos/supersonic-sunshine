#include "arealight.h"

AreaLight::AreaLight(float x, float y, float intensity){

	mLightMesh = Util::createPlaneMeshPointer(x,y);
	mProperties.points[0] = glm::vec3(x, 0.f, y);
	mProperties.points[1] = glm::vec3(x, 0.f, -y);
	mProperties.points[2] = glm::vec3(-x, 0.f, -y);
	mProperties.points[3] = glm::vec3(-x, 0.f, y);
	mProperties.intensity = intensity;

	mProperties.color = glm::vec3(1.f,1.0f,1.0f);
	mProperties.model = glm::mat4(1.0f);

	mLightMesh->setModelMatrix(mProperties.model);

	lightShader = new Shader("../shaders/simple.vert","../shaders/light.frag");

	mLightMesh->shader = lightShader;
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
	glUseProgram(*lightShader);
	glUniform3fv(glGetUniformLocation(*lightShader, "color"), 1,glm::value_ptr(mProperties.color));

	//mLightMesh.setModelMatrix(mProperties.model);
	mLightMesh->draw();
	glUseProgram(0);
}

void AreaLight::setMatrix(glm::mat4 m) {
	mLightMesh->setModelMatrix(m);
	mProperties.model = m;
}
