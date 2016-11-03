#include "SupersonicGui.h"

SupersonicGUI::SupersonicGUI(GLFWwindow* window, std::function<void(float)> cb){
// Create a nanogui screen and pass the glfw pointer to initialize

	mScreen = new Screen();
	mScreen->initialize(window, true);

	gui = new FormHelper(mScreen);
	ref<Window> nanoguiWindow = gui->addWindow(Eigen::Vector2i(0, 0), "Supersonic Sunshine");

	gui->addVariable("FPS ", fps)->setEditable(false);
	gui->addVariable("ms ", ms)->setEditable(false);

	this->addSlider("roughness", 0.f, 1.f, cb);
	mScreen->setVisible(true);
	mScreen->performLayout();

	glfwSetWindowUserPointer(window, reinterpret_cast<void*>(this));

	/** Callback lambda functions **/
	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double x, double y) {
		auto thiz = reinterpret_cast<SupersonicGUI*>(glfwGetWindowUserPointer(window));
		thiz->mScreen->cursorPosCallbackEvent(x, y);
	});

	glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int modifiers) {
		auto thiz = reinterpret_cast<SupersonicGUI*>(glfwGetWindowUserPointer(window));
		thiz->mScreen->mouseButtonCallbackEvent(button, action, modifiers);
	});

	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		auto thiz = reinterpret_cast<SupersonicGUI*>(glfwGetWindowUserPointer(window));
		thiz->mScreen->keyCallbackEvent(key, scancode, action, mods);
	});

	glfwSetCharCallback(window, [](GLFWwindow* window, unsigned int codepoint) {
		auto thiz = reinterpret_cast<SupersonicGUI*>(glfwGetWindowUserPointer(window));
		thiz->mScreen->charCallbackEvent(codepoint);
	});

	glfwSetDropCallback(window, [](GLFWwindow* window, int count, const char **filenames) {
		auto thiz = reinterpret_cast<SupersonicGUI*>(glfwGetWindowUserPointer(window));
		thiz->mScreen->dropCallbackEvent(count, filenames);
	});

	glfwSetScrollCallback(window, [](GLFWwindow *window, double x, double y) {
		auto thiz = reinterpret_cast<SupersonicGUI*>(glfwGetWindowUserPointer(window));
		thiz->mScreen->scrollCallbackEvent(x, y);
	});
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int width, int height) {
		auto thiz = reinterpret_cast<SupersonicGUI*>(glfwGetWindowUserPointer(window));
		thiz->mScreen->resizeCallbackEvent(width, height);

		Vector2i guiPosition = Vector2i(width, height) - thiz->gui->window()->size();
		guiPosition[1] = 0;
		thiz->gui->window()->setPosition(guiPosition);
	});

}

void SupersonicGUI::addSlider(std::string label, float min, float max,const std::function<void(float)> &cb){

		Slider *slider = new Slider(gui->window());
		slider->setValue(1.0f);
		slider->setFixedWidth(60);
		slider->setRange(std::pair<float,float>(min, max));
		slider->setCallback(cb);
		gui->addWidget(label, slider);

}

void SupersonicGUI::refresh(){
	gui->refresh();
}
void SupersonicGUI::draw(){
	mScreen->drawContents();
	mScreen->drawWidgets();
}

void SupersonicGUI::setFrameMetrics(int fps, float miliseconds) {
	this->fps = fps;
	this->ms = miliseconds;
}
