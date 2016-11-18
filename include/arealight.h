

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
		float intensity;
};

class AreaLight {
public:

	AreaLight(float x, float y, float intensity);
  ~AreaLight(){ delete mLightMesh; };

	void use(Shader& shader);

	void draw();


private:

	AreaLightProperties	mProperties;
	Mesh* mLightMesh;




};

#endif
