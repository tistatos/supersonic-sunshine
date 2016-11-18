#ifndef __Camera__
#define __Camera__


#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

typedef struct camera_data_t {
	glm::mat4 projection;
	glm::mat4 view;
} camera_matrices;

class Camera {
public:
	Camera(int width, int height);

	glm::vec3 mPosition;
	glm::vec3 mFacing;

	void update();

	inline glm::mat4 getViewMatrix() { return matrices.view; }
	inline void setViewMatrix(glm::mat4 view) { matrices.view = view; }

	void setProjectionMatrix(glm::mat4 proj) { matrices.projection = proj; }
	inline glm::mat4 getProjectionMatrix() { return matrices.projection; }

	inline glm::vec3 getPosition() {return mPosition;}
	inline void setPosition(glm::vec3 pos) { mPosition = pos; }

private:
	camera_matrices matrices;
	float mWidth, mHeight ;
	GLuint uboMatrices;
};

#endif
