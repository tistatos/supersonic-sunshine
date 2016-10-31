/**
* @file main.cpp
* @author Erik Sandrén
* @date 31-10-2016
*/

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

int main(int argc, char *argv[]) {
  GLFWmonitor* mMonitor;
  GLFWwindow* mWindow;

	if(!glfwInit())
		false;

  const GLFWvidmode* vidmode;
  mMonitor = glfwGetPrimaryMonitor();
  vidmode = glfwGetVideoMode(mMonitor);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	mWindow = glfwCreateWindow(vidmode->width/2, vidmode->height/2, "snopp", NULL, NULL);
	if (!mWindow) {
		glfwTerminate();
		return false;
	}
  glfwMakeContextCurrent(mWindow);
	glfwSwapInterval(0);
	glewExperimental = GL_TRUE;
	glewInit();

  while(true) {
	glClearColor(0.3f, 0.3f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glfwSwapBuffers(mWindow);
	}
	glfwTerminate();
  return 0;
}
