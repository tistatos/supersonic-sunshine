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


#include <nanogui/nanogui.h>
#include <iostream>

using namespace nanogui;

enum test_enum {
    Item1 = 0,
    Item2,
    Item3
};

bool bvar = true;
int ivar = 12345678;
double dvar = 3.1415926;
float fvar = (float)dvar;
std::string strval = "A string";
test_enum enumval = Item2;
Color colval(0.5f, 0.5f, 0.7f, 1.f);

Screen *screen = nullptr;


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

    // Create a nanogui screen and pass the glfw pointer to initialize
    screen = new Screen();
    screen->initialize(mWindow, true);
 bool enabled = true;
    FormHelper *gui = new FormHelper(screen);
    ref<Window> nanoguiWindow = gui->addWindow(Eigen::Vector2i(10, 10), "Form helper example");
    gui->addGroup("Basic types");
    gui->addVariable("bool", bvar)->setTooltip("Test tooltip.");
    gui->addVariable("string", strval);

    gui->addGroup("Validating fields");
    gui->addVariable("int", ivar)->setSpinnable(true);
    gui->addVariable("float", fvar)->setTooltip("Test.");
    gui->addVariable("double", dvar)->setSpinnable(true);

    gui->addGroup("Complex types");
    gui->addVariable("Enumeration", enumval, enabled)->setItems({ "Item 1", "Item 2", "Item 3" });
    gui->addVariable("Color", colval);

    gui->addGroup("Other widgets");
    gui->addButton("A button", []() { std::cout << "Button pressed." << std::endl; })->setTooltip("Testing a much longer tooltip, that will wrap around to new lines multiple times.");;

    screen->setVisible(true);
    screen->performLayout();
    nanoguiWindow->center();


  while (!glfwWindowShouldClose(mWindow))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        screen->drawContents();
        screen->drawWidgets();

        /* Swap front and back buffers */
        glfwSwapBuffers(mWindow);

        /* Poll for and process events */
        glfwPollEvents();
    }
	glfwTerminate();
  return 0;
}
