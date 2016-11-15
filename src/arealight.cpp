#include "arealight.h"

AreaLight::AreaLight(float x, float y, float intensity){

	mLightMesh = Util::createPlaneMeshPointer(x,y);
	mProperties.points[0] = glm::vec3(x, 0.f, y);
	mProperties.points[1] = glm::vec3(x, 0.f, -y);
	mProperties.points[2] = glm::vec3(-x, 0.f, -y);
	mProperties.points[3] = glm::vec3(-x, 0.f, y);
	mProperties.intensity = intensity;

	mProperties.model = glm::mat4(1.0f);

	//mProperties.model  = glm::rotate(mProperties.model, (float)(M_PI/2.0f), glm::vec3(1.0f,0.0f,0.0f));
	mProperties.model  = glm::translate(mProperties.model, glm::vec3(0.0f, 3.0f, 0.0f));
	mLightMesh->setModelMatrix(mProperties.model);


	Shader light("../shaders/simple.vert","../shaders/light.frag");
	std::cout << "Light shader compiled" << std::endl;
	mLightMesh->shader = &light;
}

void AreaLight::use(Shader &shader){

	glUseProgram(shader);
	GLint pointsLoc = glGetUniformLocation(shader, "arealight.points");
	GLint modelLoc = glGetUniformLocation(shader, "arealight.M");
	GLint intensityLoc = glGetUniformLocation(shader, "arealight.intensity");

	glUniform3fv(pointsLoc, 4, glm::value_ptr(mProperties.points[0]));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mProperties.model));
	glUniform1f(intensityLoc, mProperties.intensity);

	glUseProgram(0);
}

void AreaLight::draw(){
	mLightMesh->setModelMatrix(mProperties.model);
	mLightMesh->draw();
}
