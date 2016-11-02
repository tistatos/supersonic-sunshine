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

//stl 
#include <iostream>

//local
#include "SupersonicGui.h"


/* Inits a GLFW Window with OpenGL 3.3. Make sure glfwInit has been called */
GLFWwindow* createWindow(){
  GLFWmonitor* mMonitor = glfwGetPrimaryMonitor();
  const GLFWvidmode* vidmode = glfwGetVideoMode(mMonitor);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwSetErrorCallback([](int code, const char* msg){std::cout << "code: " << code << " msg: " << msg;});

	GLFWwindow* mWindow = glfwCreateWindow(vidmode->width/2, vidmode->height/2, "snopp", NULL, NULL);

	if (!mWindow) {
		std::cout << "error creating window";
		glfwTerminate();
		return nullptr;
	}

  glfwMakeContextCurrent(mWindow);
	glfwSwapInterval(0);

  return mWindow;
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

  
  //fps counter bookkeeping
  float time_since_update, time = glfwGetTime();
  int frames = 0;

  while (!glfwWindowShouldClose(mWindow)) {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    /* draw nano GUI */
    supergui->draw();

    /* Swap front and back buffers */
    glfwSwapBuffers(mWindow);

    /* Poll for and process events */
    glfwPollEvents();

    /* Fps counter handling */
    frames++;
    time = glfwGetTime();
    if (time - time_since_update > 1.0f){
      supergui->fps = frames/(time - time_since_update);
      time_since_update = time;
      supergui->refresh();
      frames = 0;
    }

  }

	glfwTerminate();
  return 0;
}
