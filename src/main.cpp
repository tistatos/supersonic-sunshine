/**
* @file main.cpp
* @author Erik Sandrén
* @date 31-10-2016
*/

//External libraries
#include <GL/glew.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#include <GLFW/glfw3.h>
#include <nanogui/nanogui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//stl
#include <iostream>

//local
#include "SupersonicGui.h"
#include "shader.h"

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

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

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

	SupersonicGUI* supergui = new SupersonicGUI(mWindow);

	cameraView = glm::lookAt(
			glm::vec3(0.f, 0.f, -4.f), glm::vec3(), glm::vec3(0.f, 1.f, 0.f)
			);

	//fps counter bookkeeping
	float time_since_update, time = glfwGetTime();
	int frames = 0;


	GLuint shader = loadShader("../shaders/simple.vert","../shaders/simple.frag");

	GLuint VAO, vbo;

	GLfloat triangle[] = 	{
				0.0f, 1.0f, 0.0f,
				0.5f, 0.0f, 0.0f,
			 -0.5f, 0.0f, 0.0f
			};

	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), triangle, GL_STATIC_DRAW );
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(mWindow)) {

		if(glfwGetKey(mWindow, GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(mWindow, GL_TRUE);

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		/* draw nano GUI */
		supergui->draw();

		glUseProgram(shader);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)0);
		glUseProgram(0);



		/* Swap front and back buffers */
		glfwSwapBuffers(mWindow);

		/* Poll for and process events */
		glfwPollEvents();

		/* Fps counter handling */
		frames++;
		time = glfwGetTime();
		if (time - time_since_update > 1.0f){
			int fps = frames/(time - time_since_update);
			float ms = (time - time_since_update);
			supergui->setFrameMetrics(fps, ms);
			time_since_update = time;
			supergui->refresh();
			frames = 0;
		}

	}

	glfwTerminate();
	return 0;
}
