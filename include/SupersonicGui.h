#ifndef SUPERGUI_H
#define SUPERGUI_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <nanogui/nanogui.h>


using namespace nanogui;

class SupersonicGUI{
public:
	SupersonicGUI(GLFWwindow* window, std::function<void(float)> cb);

	~SupersonicGUI(){
		delete mScreen;
		delete gui;
	}
	void setFrameMetrics(int fps, float miliseconds);
	void addSlider(std::string label,  float min, float max,const std::function<void(float)> &cb);
	void draw();
	void refresh();

private:
	int fps;
	float ms;
	Screen* mScreen;
	FormHelper *gui;
};

#endif
