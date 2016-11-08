

#ifndef __Camera__
#define __Camera__


#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
	Camera(int width, int height);

	void update();
	inline glm::mat4 getViewMatrix() { return mView; }
	inline glm::mat4 getProjectionMatrix() { return mProj; }

	void setProjectionMatrix(glm::mat4 proj);
	inline glm::vec3 getPosition() {return mPosition;}
	inline void setPosition(glm::vec3 pos) { mPosition = pos; updateUniform(); }

private:
	glm::mat4 mProj;
	glm::mat4 mView;
	glm::vec3 mPosition;
	float screenWidth, screenHeight;
	GLuint uboMatrices;
	void updateUniform();

};

#endif
