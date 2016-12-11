

#ifndef __AREALIGHT_H__
#define __AREALIGHT_H__

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"
#include "mesh.h"
#include "util.h"


struct AreaLightProperties {
		glm::vec3 points[4];
		glm::mat4 model;
		glm::vec3 color;
		float intensity;
};

class AreaLight {
public:
	AreaLight(float x, float y, float intensity);

	void use(Shader& shader, int n);
	void draw();
	void setMatrix(glm::mat4 m);
	void setColor(glm::vec3 color);

private:
	Mesh* mLightMesh;
	AreaLightProperties	mProperties;
	Shader* lightShader;
};

#endif
