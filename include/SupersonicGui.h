


#ifndef SUPERGUI_H
#define SUPERGUI_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <nanogui/nanogui.h>

using namespace nanogui;
class SupersonicGUI{
public: 
  SupersonicGUI(GLFWwindow *window);
  
  ~SupersonicGUI(){
    delete mScreen;
    delete gui;
  }
  
  void draw();
  void refresh();
  int fps;

private: 
  Screen* mScreen;
  FormHelper *gui;
};

#endif      