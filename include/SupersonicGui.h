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
	void setFrameMetrics(int fps, float miliseconds);
	void draw();
	void refresh();

private:
	int fps;
	float ms;
	Screen* mScreen;
	FormHelper *gui;
};

#endif
