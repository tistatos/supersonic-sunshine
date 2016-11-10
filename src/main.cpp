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
#include "ltc.h"
#include "Camera.h"

#define VSYNC true

glm::mat4 cameraProjection;
glm::mat4 cameraView;
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

	GLFWwindow* window = glfwCreateWindow(vidmode->width/2, vidmode->height/2, "snopp", NULL, NULL);

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
	float roughness = 0.5f;

	SupersonicGUI* supergui = new SupersonicGUI(mWindow, [&roughness, &shader](float val){ roughness = val; } );

	Mesh plane = Util::createPlaneMesh(100.f,100.f);


	GLuint LTCmat, LTCamp;
	glGenTextures(1, &LTCmat);
	glBindTexture(GL_TEXTURE_2D, LTCmat);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, size, size, 0, GL_RGBA, GL_FLOAT, (void*)&invM);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &LTCamp);
	glBindTexture(GL_TEXTURE_2D, LTCamp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, size, size, 0, GL_RGBA, GL_FLOAT, (void*)&tabAmplitude);
	glBindTexture(GL_TEXTURE_2D, 0);


	glClearColor(0.f,0.f,0.3f,1.0);

	//std::vector<Mesh> cornell = Util::loadFromFile("../assets/CornellBox-Empty-White.obj");
	//std::vector<Mesh> meshes = Util::loadFromFile("../assets/bunnySmall.obj");
	std::vector<Mesh> meshes;
	meshes.push_back(plane);

	glm::mat4 meshMat(1.0f);

	glm::vec3 up(0.0f, 1.0f, 0.0f);
	glm::vec3 right(1.0f, 0.0f, 0.0f);
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

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);


		glUseProgram(shader);
		camera->update();
		cameraView = camera->getViewMatrix();
		glm::mat4 mv = cameraView * meshMat;
		glm::mat3 normalMat = glm::inverseTranspose(glm::mat3(mv));
		glUniform1f(glGetUniformLocation(shader, "roughness"), roughness );

		glUniformMatrix4fv(glGetUniformLocation(shader, "m"), 1, GL_FALSE, glm::value_ptr(meshMat));
		glUniformMatrix3fv(glGetUniformLocation(shader, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMat));


		for (Mesh mesh : meshes) {
			mesh.draw();
		}

		glUseProgram(0);

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
