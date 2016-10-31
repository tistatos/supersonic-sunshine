/**
* @file main.cpp
* @author Erik Sandrén
* @date 31-10-2016
*/

#include <GL/glew.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#include <GLFW/glfw3.h>


#include <iostream>

void errorcb(int code, const char* msg){
	std::cout << "code: " << code << " msg: " << msg ; 
}

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
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	glfwSetErrorCallback(errorcb);	
	mWindow = glfwCreateWindow(vidmode->width/2, vidmode->height/2, "snopp", NULL, NULL);
	if (!mWindow) {
		std::cout << "error creating window";
		glfwTerminate();
		return false;
	}
  glfwMakeContextCurrent(mWindow);
	glfwSwapInterval(0);
	glewExperimental = GL_TRUE;
	glewInit();

  while (!glfwWindowShouldClose(mWindow))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(mWindow);

        /* Poll for and process events */
        glfwPollEvents();
    }
	glfwTerminate();
  return 0;
}
