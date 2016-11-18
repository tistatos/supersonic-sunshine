/**
* @file main.cpp
* @author Erik Sandrén
* @date 31-10-2016
*/

//External libraries
#include <GL/glew.h>

#ifdef __APPLE__

#else
	#include <GL/gl.h>
#endif
#include <GLFW/glfw3.h>
//#include <nanogui/nanogui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure

//stl
#include <iostream>
#include <fstream>
#include <vector>

//local
#include "SupersonicGui.h"
#include "shader.h"
#include "mesh.h"
#include "util.h"
#include "ltcMaps.h"
#include "Camera.h"

#define VSYNC true

Camera *camera;

void windowResizedCallback(GLFWwindow* window, int width, int height) {
	float aspect = (float)width/height;
	camera->setProjectionMatrix(glm::perspective((float)M_PI/3.0f, aspect, 0.001f, 1000.0f));
}

/* Inits a GLFW Window with OpenGL 3.3. Make sure glfwInit has been called */
GLFWwindow* createWindow(){
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwSetErrorCallback([](int code, const char* msg){std::cout << "code: " << code << " msg: " << msg;});

	GLFWwindow* window = glfwCreateWindow(vidmode->width/2, vidmode->height/2, "SuperSonicSunshine", NULL, NULL);

	if (!window) {
		std::cout << "error creating window";
		glfwTerminate();
		return nullptr;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(VSYNC);

	glfwSetWindowSizeCallback(window, windowResizedCallback);
	return window;
}


int main(int argc, char *argv[]) {
	if(!glfwInit())
		return -1;

	GLFWwindow* mWindow = createWindow();
	if (mWindow == nullptr)
		return -1;

	glewExperimental = GL_TRUE;
	glewInit();
	printf("GL Vendor:			%s\n", glGetString(GL_VENDOR));
	printf("GL Render:			%s\n", glGetString(GL_RENDERER));
	printf("GL Version:			%s\n", glGetString(GL_VERSION));

	int width, height;
	glfwGetWindowSize(mWindow, &width, &height);
	camera = new Camera(width, height);
	camera->update();

	//fps counter bookkeeping
	float time_since_update, time, lastFrame = glfwGetTime();
	int frames = 0;


	Shader shader("../shaders/simple.vert","../shaders/simple.frag");
	Shader light("../shaders/simple.vert","../shaders/light.frag");

	float roughness = 0.5f;

	SupersonicGUI* supergui = new SupersonicGUI(mWindow, [&roughness, &shader](float val){ roughness = val; } );

	Mesh plane = Util::createPlaneMesh(10.f, 10.f);
	plane.shader = &shader;

	Mesh lightPlane = Util::createPlaneMesh(2.f, 2.f);
	lightPlane.shader = &light;

	LTC_t maps = loadLTCTextures();

	glClearColor(0.f,0.f,0.3f,1.0);

	std::vector<Mesh*> meshes;
	meshes.push_back(&plane);
	meshes.push_back(&lightPlane);

	glm::vec3 up(0.0f, 1.0f, 0.0f);
	glm::vec3 right(1.0f, 0.0f, 0.0f);

	glm::mat4 meshMat(1.0f);

	glm::mat4 lightMat(1.0f);
	lightMat = glm::rotate(lightMat, (float)(M_PI/2), right);
	lightMat = glm::translate(lightMat, glm::vec3(0.0f, -3.0f, -2.5f));

	lightPlane.setModelMatrix(lightMat);

	float delta = 0.0f;
	while (!glfwWindowShouldClose(mWindow)) {
		time = glfwGetTime();
		delta = time - lastFrame;
		if(glfwGetKey(mWindow, GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(mWindow, GL_TRUE);
		if(glfwGetKey(mWindow, GLFW_KEY_A))
			meshMat = glm::rotate(meshMat, (float)(delta * M_PI/3.0f), up);
		if(glfwGetKey(mWindow, GLFW_KEY_D))
			meshMat = glm::rotate(meshMat, (float)(delta * -M_PI/3.0f), up);
		if(glfwGetKey(mWindow, GLFW_KEY_W))
			meshMat = glm::rotate(meshMat, (float)(delta * M_PI/3.0f), right);
		if(glfwGetKey(mWindow, GLFW_KEY_S))
			meshMat = glm::rotate(meshMat, (float)(delta * -M_PI/3.0f), right);
		if(glfwGetKey(mWindow, GLFW_KEY_Q))
			meshMat = glm::scale(meshMat, glm::vec3(1.1f));
		if(glfwGetKey(mWindow, GLFW_KEY_E))
			meshMat = glm::scale(meshMat, glm::vec3(0.9f));

		glm::mat4 cameraMat = camera->getViewMatrix();

		cameraMat = glm::translate(cameraMat, glm::vec3(0.0f, 1.0f, 6.0f));
		if(glfwGetKey(mWindow, GLFW_KEY_UP)) {
			cameraMat = glm::rotate(cameraMat, (float)(delta * M_PI/3.0f), right);
		}
		if(glfwGetKey(mWindow, GLFW_KEY_DOWN)) {
			cameraMat = glm::rotate(cameraMat, (float)(delta * -M_PI/3.0f), right);
		}
		cameraMat = glm::translate(cameraMat, glm::vec3(0.0f, -1.0f, -6.0f));

		camera->setViewMatrix(cameraMat);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

		camera->update();

		plane.setModelMatrix(meshMat);
		plane.setRoughness(roughness);

		bindTextures(&maps);

		for (Mesh* mesh : meshes) {
			mesh->draw();
		}

		supergui->draw();

		glfwSwapBuffers(mWindow);
		glfwPollEvents();

		/* Fps counter handling */
		frames++;
		if (time - time_since_update > 1.0f){
			int fps = frames/(time - time_since_update);
			float ms = 1000.0f /frames;
			supergui->setFrameMetrics(fps, ms);
			time_since_update = time;
			supergui->refresh();
			frames = 0;
		}
		lastFrame = time;
	}

	glfwTerminate();
	return 0;
}
