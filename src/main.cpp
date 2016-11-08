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

#define VSYNC true

glm::mat4 cameraProjection;
glm::mat4 cameraView;

void windowResizedCallback(GLFWwindow* window, int width, int height) {
	float aspect = (float)width/height;
	cameraProjection = glm::perspective((float)M_PI/3.0f, aspect, 0.001f, 1000.0f);
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

	int width, height;
	glfwGetWindowSize(window, &width, &height);

	float aspect = (float)width/height;
	cameraProjection = glm::perspective((float)M_PI/3.0f, aspect, 0.001f, 1000.0f);
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


	cameraView = glm::lookAt(
			glm::vec3(0.f, 1.5f, 6.f), //position
			glm::vec3(0.f, 0.5f, 0.f),  // look at
			glm::vec3(0.f, 1.f, 0.f) // up-vector
			);

	//fps counter bookkeeping
	float time_since_update, time, lastFrame = glfwGetTime();
	int frames = 0;


	Shader shader("../shaders/simple.vert","../shaders/simple.frag");
	float roughness = 0.5f;
	float scale = 0.3f;
	SupersonicGUI* supergui = new SupersonicGUI(mWindow, [&roughness, &shader](float val){ roughness = val; } );
	Mesh* mesh = Util::createTriangleMesh();



	//std::vector<Mesh> cornell = Util::loadFromFile("../assets/CornellBox-Empty-White.obj");

	std::vector<Mesh> meshes = Util::loadFromFile("../assets/sphere.obj");
	glm::mat4 meshMat(1.0);

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
			scale += 0.1*delta;
		if(glfwGetKey(mWindow, GLFW_KEY_E))
			scale -= 0.1*delta;
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

		glClearColor(0.f,0.f,0.3f,1.0);
		/* draw nano GUI */

		glUseProgram(shader);
		glm::mat4 mvp = cameraProjection * cameraView * meshMat;
		glm::mat4 mv = cameraView * meshMat;
		glUniform1f(glGetUniformLocation(shader, "roughness"), roughness );
		glUniform1f(glGetUniformLocation(shader, "scale"), scale );
		glUniformMatrix4fv(glGetUniformLocation(shader, "mv"), 1, GL_FALSE, glm::value_ptr(mv));
		glUniformMatrix4fv(glGetUniformLocation(shader, "mvp"), 1, GL_FALSE, glm::value_ptr(mvp));
		for (Mesh mesh : meshes) {
			mesh.draw();
		}
		// for (Mesh mesh : cornell){
		// 	mesh.draw();
		// }
		glUseProgram(0);

		supergui->draw();
		/* Swap front and back buffers */
		glfwSwapBuffers(mWindow);

		/* Poll for and process events */
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
